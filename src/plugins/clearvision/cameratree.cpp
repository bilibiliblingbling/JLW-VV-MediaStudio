#include <QCoreApplication>

#include <projectexplorer/basictreewidget.h>
#include <utils/algorithm.h>
#include <utils/tmplog.h>

#include "cameratree.h"
#include "clearvisionnodes.h"
#include "constants.h"
#include "devices.h"
#include "mediasource.h"

namespace ClearVision {
namespace Internal {

static CameraTree *s_cameratree = 0;

CameraTree::CameraTree(QObject *parent)
    : MS::CameraTree(parent)
    , m_virtualRootNode(0)
    , m_rootNode(0)
    , m_currentNode(0)
    , m_currentMediaSource(0)
    , m_parentNodeForChange(0)
    , m_objectAdded(0)
{
    setObjectName(QLatin1String("ClearVision.CameraTree"));

    m_rootNode = new RootNode();

    m_virtualRootNode = new PE::VirtualFolderNode(Constants::CAMERA_VIRTUALROOTNODE);
    m_virtualRootNode->addSubFolderNode(m_rootNode);

    s_cameratree = this;
}

CameraTree::~CameraTree()
{
    s_cameratree = 0;

    delete m_virtualRootNode;
}

CameraTree *CameraTree::instance()
{
    return s_cameratree;
}

void CameraTree::setCurrentMediaSource(PE::MediaSource *mediasource)
{
    if (MediaSource *cvms = qobject_cast<MediaSource *>(mediasource))
        update(0, cvms);
}

ProjectExplorer::FolderNode *CameraTree::rootNode()
{
    return m_virtualRootNode;
}

ProjectExplorer::Node *CameraTree::currentNode()
{
    return m_currentNode;
}

void CameraTree::nodeChanged(ProjectExplorer::BasicTreeWidget *widget)
{
    // TODO: check widget
    if (widget) {
        update(widget->currentNode(), m_currentMediaSource);
    }
}

bool CameraTree::canFetchMoreInTree(ProjectExplorer::FolderNode *folder) const
{
    if (ObjectNode *ono = dynamic_cast<ObjectNode *>(folder)) {
        return ono->isCamera();
    }

    return false;
}

void CameraTree::aboutToFetchMore(ProjectExplorer::FolderNode *folder)
{
    // check whether can fetch more or not
    if (!m_currentMediaSource || folder->asNode<ObjectNode>()->isCamera())
        return;

    // FIXME: check whether chilren are already fetched
    if (m_folderNodes.contains(folder->id()))
        return;

    // root node, unit node, domain node
    if (m_currentMediaSource->shouldFetchMore(folder->id())) {
        m_folderNodes[folder->id()] = dynamic_cast<ObjectNode *>(folder);
        m_currentMediaSource->aboutTofetchMore(folder->metadata());
    } else {
        objectsAddedImpl(folder, m_currentMediaSource->childObjects(folder->id()));
    }
}

bool CameraTree::selectableInTree(ProjectExplorer::Node *node) const
{
    return (node && node->asNode<ObjectNode>() && node->asNode<ObjectNode>()->isCamera());
}

bool CameraTree::showInTree(ProjectExplorer::Node *node) const
{
    if (StatusNode *n = node->asNode<StatusNode>())
        return n->showInTree();

    return true;
}

void CameraTree::update(ProjectExplorer::Node *node, MediaSource *currentMediaSource)
{
    bool mediaSourceChanged = (m_currentMediaSource != currentMediaSource);
    bool nodeChanged = (m_currentNode != node);

    if (mediaSourceChanged) {
        if (m_currentMediaSource) {
            disconnect(m_currentMediaSource, 0, this, 0);

            objectsAboutToBeCleared(0);
        }

        m_currentMediaSource = currentMediaSource;

        if (m_currentMediaSource) {
            // add object
            connect(m_currentMediaSource, &MediaSource::objectAboutToBeAdded,
                    this, &CameraTree::objectAboutToBeAdded);
            connect(m_currentMediaSource, &MediaSource::objectAdded,
                    this, &CameraTree::objectAdded);

            // add objects
            connect(m_currentMediaSource, &MediaSource::objectsAboutToBeAdded,
                    this, &CameraTree::objectsAboutToBeAdded);
            connect(m_currentMediaSource, &MediaSource::objectsAdded,
                    this, &CameraTree::objectsAdded);

            // remove object
            connect(m_currentMediaSource, &MediaSource::objectAboutToBeRemoved,
                    this, &CameraTree::objectAboutToBeRemoved);
            connect(m_currentMediaSource, &MediaSource::objectRemoved,
                    this, &CameraTree::objectRemoved);

            // remove objects
            connect(m_currentMediaSource, &MediaSource::objectsAboutToBeCleared,
                    this, &CameraTree::objectsAboutToBeCleared);
            connect(m_currentMediaSource, &MediaSource::objectsCleared,
                    this, &CameraTree::objectsCleared);

            // for status node
            connect(m_currentMediaSource, &MediaSource::fetchingMore,
                    this, &CameraTree::fetchingMore);
            connect(m_currentMediaSource, &MediaSource::fetchMoreFailed,
                    this, &CameraTree::fetchMoreFailed);
            connect(m_currentMediaSource, &MediaSource::fetchMoreSucceed,
                    this, &CameraTree::fetchMoreSucceed);

            connect(m_currentMediaSource, &MediaSource::destroyed,
                    this, [this]() { update(0, 0); });

            aboutToFetchMore(m_rootNode);
        }
    }

    if (nodeChanged) {
        m_currentNode = node;
    }
}

void CameraTree::objectAboutToBeAdded(UObject *obj)
{
    m_objectAdded = obj;
}

void CameraTree::objectAdded()
{
    if (m_objectAdded && m_objectAdded->isValid())
        objectAddedImpl(m_objectAdded);
}

void CameraTree::objectAboutToBeRemoved(UObject *obj)
{
    // TODO: remove object
}

void CameraTree::objectRemoved()
{
    // nothing to do
}

void CameraTree::objectsAboutToBeAdded(Core::Id unitId, const QList<UObject *> &objects)
{
    // TODO:
    Q_UNUSED(objects);

    m_parentNodeForChange = folderOfId(m_rootNode, unitId);
}

void CameraTree::objectsAdded()
{
    if (m_parentNodeForChange && m_parentNodeForChange->isValid()) {
        objectsAddedImpl(m_parentNodeForChange, m_currentMediaSource->childObjects(m_parentNodeForChange->id()));
    }

    m_parentNodeForChange = 0;
}

/*!
 * \brief CameraTree::objectsAboutToBeCleared
 * \param unit
 *  清理
 */
void CameraTree::objectsAboutToBeCleared(UObject *obj)
{
    if (obj) {
        // TODO: remove folder, and clear cache
    } else {
        m_rootNode->removeAllSubFolderNodes();
        m_folderNodes.clear();
    }
#if 0
    // do not remove children, which are status nodes
    m_rootNode->removeAllNodes();
#endif
}

void CameraTree::objectsCleared()
{
    // nothing to do;
}

void CameraTree::fetchingMore(Core::Id unitId)
{
    if (PE::FolderNode *folder = folderOfId(m_rootNode, unitId)) {
        if (ObjectNode *ono = dynamic_cast<ObjectNode *>(folder)) {
            ono->fetchingMore();
        }
    }
}

void CameraTree::fetchMoreFailed(Core::Id unitId, const QString &error)
{
    if (PE::FolderNode *folder = folderOfId(m_rootNode, unitId)) {
        if (ObjectNode *ono = dynamic_cast<ObjectNode *>(folder)) {
            ono->fetchMoreFailed(error);
        }
    }
}

void CameraTree::fetchMoreSucceed(Core::Id unitId)
{
    if (PE::FolderNode *folder = folderOfId(m_rootNode, unitId)) {
        if (ObjectNode *ono = dynamic_cast<ObjectNode *>(folder)) {
            ono->fetchMoreSucceed();
        }
    }
}

void CameraTree::objectAddedImpl(UObject *obj)
{
#if 0
    ObjectNode *ono = createObjectNode(obj);

    TM_DBG << "unit id:" << ono->unitId().toString().toStdString();
    PE::FolderNode *folder = folderOfId(m_virtualRootNode->subFolderNodes().front(), ono->unitId());
    if (folder) {
        folder->addSubFolderNode(ono);
    }
#endif
}

void CameraTree::objectsAddedImpl(ProjectExplorer::FolderNode *folder, const QList<UObject *> &objects)
{
#if 0
    QList<PE::FolderNode *> folderNodes;
    foreach (auto obj, objects) {
        ObjectNode *ono = createObjectNode(obj);
        folderNodes.append(ono);
    }

    // TODO: cannot show properly when use addSubFolderNodes
    folder->addSubFolderNodes(folderNodes);
#else
    TM_DBG << objects.size();
    foreach (auto obj, objects) {
        TM_DBG("list object, id: %s, name: %s", obj->objId().getData(), obj->objName().toStdString().c_str());
    }

    foreach (auto obj, objects) {
        TM_DBG("new object node, id: %s, name: %s", obj->objId().getData(), obj->objName().toStdString().c_str());
        ObjectNode *ono = obj->createNode();
        folder->addSubFolderNode(ono);
    }
#endif
}

PE::FolderNode *CameraTree::folderOfId(ProjectExplorer::FolderNode *startFolder, Core::Id id)
{
    if (m_folderNodes.contains(id))
        return m_folderNodes.value(id);

    if (startFolder->id() == id) {
        if (ObjectNode *ono = dynamic_cast<ObjectNode *>(startFolder))
            m_folderNodes.insert(id, ono);

        return startFolder;
    }

    foreach (PE::FolderNode *folder, startFolder->subFolderNodes()) {
        if (PE::FolderNode *f = folderOfId(folder, id)) {
            if (ObjectNode *ono = dynamic_cast<ObjectNode *>(f))
                m_folderNodes.insert(id, ono);

            return f;
        }
    }

    return 0;
}

} // namespace Internal
} // namespace ClearVision
