#include <QAction>
#include <QMenu>

#include <core/icore.h>
#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>

#include <projectexplorer/basictreewidget.h>
#include <projectexplorer/camera.h>
#include <projectexplorer/mediasource.h>
#include <projectexplorer/segment.h>
#include <resource/resource.h>
#include <utils/algorithm.h>
#include <utils/tmplog.h>

#include "cameranode.h"
#include "constants.h"
#include "segmentnode.h"
#include "segmenttree.h"

namespace MediaSource {
namespace Internal {

static SegmentTree *s_segmenttree = 0;

SegmentTree::SegmentTree(QObject *parent)
    : BasicTree(parent)
    , m_virtualRootNode(0)
    , m_rootNode(0)
    , m_currentNode(0)
    , m_currentMediaSource(0)
    , m_segmentAdded(0)
{
    setObjectName(QStringLiteral("MediaSource.SegmentTree"));

    m_rootNode = new PE::VirtualFolderNode(Constants::SEGMENT_ROOTNODE, tr("Query Results"));
    m_rootNode->setIcon(Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons,  Resource::Constants::IDX_HOME));

    m_virtualRootNode = new PE::VirtualFolderNode(Constants::SEGMENT_VIRTUALROOTNODE);
    m_virtualRootNode->addSubFolderNode(m_rootNode);

    ////////////////////////////////////////////////////////////////////////////
    // setup menus
    Core::Context segmentcontext(Constants::C_MEDIASOURCE_BROWSEPANEWIDGET);

    Core::Command *cmd = 0;
    Core::Command *removecmd = 0;

    // segments root
    Core::ActionContainer *segmentrootemenu
            = Core::ActionManager::createMenu(Constants::M_SEGMENT_SEGMENTROOT);
    segmentrootemenu->appendGroup(Constants::G_SEGMENT_CLEAR);
    m_clearAction = new QAction(tr("Clear"), this);
    cmd = Core::ActionManager::registerAction(m_clearAction, Constants::SEGMENT_CLEAR, segmentcontext);
    segmentrootemenu->addAction(cmd, Constants::G_SEGMENT_CLEAR);

    // camera
    Core::ActionContainer *cameramenu
            = Core::ActionManager::createMenu(Constants::M_SEGMENT_CAMERA);
    cameramenu->appendGroup(Constants::G_SEGMENT_REMOVE);
    m_removeAction = new QAction(tr("Remove"), this);
    removecmd = Core::ActionManager::registerAction(m_removeAction, Constants::SEGMENT_REMOVE, segmentcontext);
    cameramenu->addAction(removecmd, Constants::G_SEGMENT_REMOVE);

    // segment
    Core::ActionContainer *segmentmenu
            = Core::ActionManager::createMenu(Constants::M_SEGMENT_SEGEMENT);
    segmentmenu->appendGroup(Constants::G_SEGMENT_REMOVE);
    segmentmenu->appendGroup(Constants::G_SEGMENT_PLAY);
    segmentmenu->appendGroup(Constants::G_SEGMENT_ADDTO);

    segmentmenu->addSeparator(Constants::G_SEGMENT_REMOVE);
    segmentmenu->addSeparator(Constants::G_SEGMENT_PLAY);
    segmentmenu->addSeparator(Constants::G_SEGMENT_ADDTO);

    segmentmenu->addAction(removecmd, Constants::G_SEGMENT_REMOVE);

    m_playInMonitorAction = new QAction(tr("Monitor"), this);
    cmd = Core::ActionManager::registerAction(m_playInMonitorAction, Constants::SEGMENT_PLAYINMONITOR, segmentcontext);
    segmentmenu->addAction(cmd, Constants::G_SEGMENT_PLAY);

    Core::ActionContainer *fastpreviewmenu
            = Core::ActionManager::createMenu(Constants::M_SEGMENT_FASTPREVIEW);
    fastpreviewmenu->setOnAllDisabledBehavior(Core::ActionContainer::Show);
    segmentmenu->addMenu(fastpreviewmenu, Constants::G_SEGMENT_PLAY);
    m_playInPreviewMenu = fastpreviewmenu->menu();
    m_playInPreviewMenu->setTitle(tr("Fast Preview"));

    m_addToRepositoryAction = new QAction(tr("Add to Segment Repository"), this);
    cmd = Core::ActionManager::registerAction(m_addToRepositoryAction, Constants::SEGMENT_ADDTOREPOSITORY, segmentcontext);
    segmentmenu->addAction(cmd, Constants::G_SEGMENT_ADDTO);

    m_addToSequenceAction = new QAction(tr("Add to Sequence"), this);
    cmd = Core::ActionManager::registerAction(m_addToSequenceAction, Constants::SEGMENT_ADDTOSEQUENCE, segmentcontext);
    segmentmenu->addAction(cmd, Constants::G_SEGMENT_ADDTO);

    // setup connections
    connect(m_clearAction, &QAction::triggered, this, [this]() {
        if (m_currentMediaSource) {
            m_currentMediaSource->clearSegments();
        }
    });

    connect(m_removeAction, &QAction::triggered, this, [this]() {
        if (m_currentMediaSource
            && m_currentNode && m_currentNode->isValid()) {
            if (CameraNode *cn = m_currentNode->asNode<CameraNode>()) {
                m_currentMediaSource->clearCameraSegments(cn->id());
            } else if (SegmentNode *sn = m_currentNode->asNode<SegmentNode>()) {
                m_currentMediaSource->removeSegment(sn->id());
            }
        }
    });

    connect(m_playInMonitorAction, &QAction::triggered, this, [this]() {
        // TODO:
    });

    connect(m_playInPreviewMenu, &QMenu::aboutToShow, this, &SegmentTree::updateFastPreviewMenu);

    connect(m_addToRepositoryAction, &QAction::triggered, this, [this]() {
        // TODO:
    });

    connect(m_addToSequenceAction, &QAction::triggered, this, [this]() {
        // TODO:
    });

    s_segmenttree = this;
}

SegmentTree::~SegmentTree()
{
    s_segmenttree = 0;

    delete m_virtualRootNode;
}

SegmentTree *SegmentTree::instance()
{
    return s_segmenttree;
}

void SegmentTree::setCurrentMediaSource(PE::MediaSource *currentMediaSource)
{
    s_segmenttree->update(0, currentMediaSource);
}

PE::FolderNode *SegmentTree::rootNode()
{
    return m_virtualRootNode;
}

PE::Node *SegmentTree::currentNode()
{
    return m_currentNode;
}

void SegmentTree::nodeChanged(PE::BasicTreeWidget *widget)
{
    if (widget)
        update(widget->currentNode(), m_currentMediaSource);
}

void SegmentTree::showContextMenu(PE::BasicTreeWidget *focus, const QPoint &globalPos, PE::Node *node)
{
    Q_UNUSED(focus);

    if (!node)
        return;

    emit aboutToShowContextMenu(node);

    QMenu *contextMenu = 0;

    if (node == m_rootNode) {
        contextMenu = Core::ActionManager::actionContainer(Constants::M_SEGMENT_SEGMENTROOT)->menu();
    } else if (node->asNode<CameraNode>()) {
        contextMenu = Core::ActionManager::actionContainer(Constants::M_SEGMENT_CAMERA)->menu();
    } else if (node->asNode<SegmentNode>()) {
        contextMenu = Core::ActionManager::actionContainer(Constants::M_SEGMENT_SEGEMENT)->menu();
    } else {
        TM_DBG << "invalid node:" << node->displayName().toStdString();
    }

    if (contextMenu && (contextMenu->actions().count() > 0)) {
        contextMenu->popup(globalPos);
    }
}

bool SegmentTree::dragableInTree(PE::Node *node) const
{
    return (node->asNode<SegmentNode>());
}

void SegmentTree::update(PE::Node *node, PE::MediaSource *currentMediaSource)
{
    bool mediaSourceChanged = (m_currentMediaSource != currentMediaSource);
    bool nodeChanged = (m_currentNode != node);

    if (mediaSourceChanged) {
        if (m_currentMediaSource) {
            disconnect(m_currentMediaSource, 0, this, 0);

            segmentsAboutToBeCleared();
        }

        m_currentMediaSource = currentMediaSource;

        if (m_currentMediaSource) {
            // setup signals and slots
            connect(m_currentMediaSource, &PE::MediaSource::segmentAboutToBeAdded,
                    this, &SegmentTree::segmentAboutToBeAdded);
            connect(m_currentMediaSource, &PE::MediaSource::segmentAdded,
                    this, &SegmentTree::segmentAdded);

            connect(m_currentMediaSource, &PE::MediaSource::segmentsAboutToBeCleared,
                    this, &SegmentTree::segmentsAboutToBeCleared);
            connect(m_currentMediaSource, &PE::MediaSource::segmentsCleared,
                    this, &SegmentTree::segmentsCleared);

            connect(m_currentMediaSource, &PE::MediaSource::cameraSegmentsAboutToBeCleared,
                    this, &SegmentTree::cameraSegmentsAboutToBeCleared);
            connect(m_currentMediaSource, &PE::MediaSource::cameraSegmentsCleared,
                    this, &SegmentTree::cameraSegmentsCleared);

            connect(m_currentMediaSource, &PE::MediaSource::segmentAboutToBeRemoved,
                    this, &SegmentTree::segmentAboutToBeRemoved);
            connect(m_currentMediaSource, &PE::MediaSource::segmentRemoved,
                    this, &SegmentTree::segmentRemoved);

            connect(m_currentMediaSource, &PE::MediaSource::destroyed,
                    this, [this]() { update(0, 0); });

            // load previous queried segments results
            foreach (auto segment, m_currentMediaSource->segments()) {
                segmentAddedImpl(segment);
            }
        }
    }

    if (nodeChanged) {
        m_currentNode = node;
        // TODO: check whether node change signal is needed
    }
}

void SegmentTree::updateFastPreviewMenu()
{
    m_playInPreviewMenu->clear();
    // TODO: generate submenu
    m_playInPreviewMenu->addAction(tr("TODO: generate based on fast preview info"));
}

void SegmentTree::segmentAboutToBeAdded(PE::Segment *segment)
{
    m_segmentAdded = segment;
}

void SegmentTree::segmentAdded()
{
    if (m_segmentAdded && m_segmentAdded->isValid())
        segmentAddedImpl(m_segmentAdded);

    m_segmentAdded = 0;
}

void SegmentTree::segmentAboutToBeRemoved(PE::Segment *segment)
{
    if (!segment || !segment->isValid())
        return;

    PE::FolderNode *folder
            = Utils::findOr(m_rootNode->subFolderNodes(), 0,
                            [segment](PE::FolderNode *f) -> bool { return (f->id() == segment->cameraId()); });

    if (folder) {
        foreach (PE::Node *node, folder->childNodes()) {
            if (node->id() == segment->id()) {
                folder->removeNode(node);
                break;
            }
        }
    }

    // cleanExpiredFolders();
}

void SegmentTree::segmentRemoved()
{
    // nothing to do
}

void SegmentTree::cameraSegmentsAboutToBeCleared(PE::Camera *camera)
{
    if (!camera || !camera->isValid())
        return;

    PE::FolderNode *folder
            = Utils::findOr(m_rootNode->subFolderNodes(), 0,
                            [camera](PE::FolderNode *f) -> bool { return (f->id() == camera->id()); });

    if (folder) {
        m_rootNode->removeSubFolderNode(folder);
    }
}

void SegmentTree::cameraSegmentsCleared()
{
    // nothing to do
}

void SegmentTree::segmentsAboutToBeCleared()
{
    m_rootNode->removeAllSubFolderNodes();
    m_rootNode->removeAllNodes();
}

void SegmentTree::segmentsCleared()
{
    // nothing to do
}

void SegmentTree::segmentAddedImpl(PE::Segment *segment)
{
    // TODO: optimization reuired
    PE::FolderNode *folder
            = Utils::findOr(m_rootNode->subFolderNodes(), 0, [segment](PE::FolderNode *f) -> bool { return (f->id() == segment->cameraId()); });

    if (!folder) {
        PE::Camera *camera = m_currentMediaSource->cameraOfId(segment->cameraId());
        if (camera) {
            folder = new CameraNode(camera);
            m_rootNode->addSubFolderNode(folder);
        } else {
            TM_DBG("failed to get camera of id:") << segment->cameraId().toString().toStdString();
            return;
        }
    }

    folder->addNode(new SegmentNode(segment));
}

} // namespace Internal
} // namespace MediaSource
