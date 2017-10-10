#include <projectexplorer/basicnodes.h>
#include <projectexplorer/basictreewidget.h>
#include <resource/resource.h>
#include <utils/algorithm.h>

#include "cameratree.h"
#include "constants.h"
#include "devices.h"
#include "kingvisionnodes.h"
#include "mediasource.h"

namespace KingVision {
namespace Internal {

namespace PE = ProjectExplorer;

static CameraTree *s_cameratree = 0;

CameraTree::CameraTree()
    : m_virtualRootNode(0)
    , m_rootNode(0)
    , m_currentNode(0)
    , m_currentMediaSource(0)
    , m_cameraAdded(0)
{
    setObjectName(QLatin1String("KingVision.CameraTree"));

    m_rootNode = new PE::VirtualFolderNode(Constants::KINGVISION_CAMERA_ROOTNODE, tr("Cameras"));
    m_rootNode->setIcon(QIcon(Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, Resource::Constants::IDX_FOLDER)));

    m_virtualRootNode = new PE::VirtualFolderNode(Constants::KINGVISION_CAMERA_VIRTUALROOT_NODE);
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
    if (MediaSource *kvms = qobject_cast<MediaSource *>(mediasource))
        update(0, kvms);
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
    if (widget) {
        update(widget->currentNode(), m_currentMediaSource);
    }
}

void CameraTree::update(ProjectExplorer::Node *node, MediaSource *currentMediaSource)
{
    bool mediaSourceChanged = (m_currentMediaSource != currentMediaSource);
    bool nodeChanged = (m_currentNode != node);

    if (mediaSourceChanged) {
        if (m_currentMediaSource) {
            disconnect(m_currentMediaSource, 0, this, 0);

            // clean old devices
            camerasAboutToBeCleared();
        }

        m_currentMediaSource = currentMediaSource;

        if (m_currentMediaSource) {
            connect(m_currentMediaSource, &MediaSource::cameraAboutToBeAdded,
                    this, &CameraTree::cameraAboutToBeAdded);
            connect(m_currentMediaSource, &MediaSource::cameraAdded,
                    this, &CameraTree::cameraAdded);

            connect(m_currentMediaSource, &MediaSource::cameraAboutToBeRemoved,
                    this, &CameraTree::cameraAboutToBeRemoved);
            connect(m_currentMediaSource, &MediaSource::cameraRemoved,
                    this, &CameraTree::cameraRemoved);

            connect(m_currentMediaSource, &MediaSource::camerasAboutToBeCleared,
                    this, &CameraTree::camerasAboutToBeCleared);
            connect(m_currentMediaSource, &MediaSource::camerasCleared,
                    this, &CameraTree::camerasCleared);

            connect(m_currentMediaSource, &MediaSource::destroyed,
                    this, [this]() { update(0, 0); });

            foreach (auto camera, m_currentMediaSource->cameras()) {
                cameraAddedImpl(camera);
            }
        }
    }

    if (nodeChanged) {
        m_currentNode = node;
    }
}

void CameraTree::cameraAboutToBeAdded(Camera *camera)
{
    m_cameraAdded = camera;
}

void CameraTree::cameraAdded()
{
    if (m_cameraAdded && m_cameraAdded->isValid())
        cameraAddedImpl(m_cameraAdded);

    m_cameraAdded = 0;
}

void CameraTree::cameraAboutToBeRemoved(Camera *camera)
{
    if (camera && camera->isValid()) {
        PE::FolderNode *folder
                = Utils::findOr(m_rootNode->subFolderNodes(), 0, [camera](PE::FolderNode *f) -> bool { return (f->id() == camera->id()); });
        if (folder) {
            m_rootNode->removeSubFolderNode(folder);
        }
    }
}

void CameraTree::cameraRemoved()
{
    // nothing to do
}

void CameraTree::camerasAboutToBeCleared()
{
    m_rootNode->removeAllSubFolderNodes();
    m_rootNode->removeAllNodes();
}

void CameraTree::camerasCleared()
{
    // nothing to do
}

void CameraTree::cameraAddedImpl(Camera *camera)
{
    m_rootNode->addSubFolderNode(new CameraNode(camera));
}

} // namespace Internal
} // namespace KingVision
