#ifndef MEDIASOURCE_SEGMENT_TREE_H
#define MEDIASOURCE_SEGMENT_TREE_H

#include <projectexplorer/basictree.h>


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

namespace ProjectExplorer {
class MediaSource;
class Segment;
class Camera;
}

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class SegmentTree : public PE::BasicTree
{
    Q_OBJECT

public:
    explicit SegmentTree(QObject *parent = 0);
    ~SegmentTree();

    static SegmentTree *instance();

public:
    static void setCurrentMediaSource(PE::MediaSource *currentMediaSource);

public:
    PE::FolderNode *rootNode() Q_DECL_OVERRIDE;
    PE::Node *currentNode() Q_DECL_OVERRIDE;

    void nodeChanged(PE::BasicTreeWidget *widget) Q_DECL_OVERRIDE;
    void showContextMenu(PE::BasicTreeWidget *focus, const QPoint &globalPos, PE::Node *node) Q_DECL_OVERRIDE;
    bool dragableInTree(PE::Node *node) const Q_DECL_OVERRIDE;

private:
    void update(PE::Node *node, PE::MediaSource *currentMediaSource);

private:
    void updateFastPreviewMenu();

private:
    void segmentAboutToBeAdded(PE::Segment *segment);
    void segmentAdded();

    void segmentAboutToBeRemoved(PE::Segment *segment);
    void segmentRemoved();

    void cameraSegmentsAboutToBeCleared(PE::Camera *camera);
    void cameraSegmentsCleared();

    void segmentsAboutToBeCleared();
    void segmentsCleared();

private:
    void segmentAddedImpl(PE::Segment *segment);

private:
    PE::VirtualFolderNode *m_virtualRootNode;   // parent node of toplevel items
    PE::VirtualFolderNode *m_rootNode;          // parent node of all cameras

private:
    PE::Node *m_currentNode;
    PE::MediaSource *m_currentMediaSource;

private:
    PE::Segment *m_segmentAdded;

private:
    // segment related actions
    QAction *m_clearAction;
    QAction *m_removeAction;
    QAction *m_playInMonitorAction;
    QAction *m_addToRepositoryAction;
    QAction *m_addToSequenceAction;

    QMenu *m_playInPreviewMenu;
};

} // namespace Internal
} // namespace MediaSource

#endif // MEDIASOURCE_SEGMENT_TREE_H
