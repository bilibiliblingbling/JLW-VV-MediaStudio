#ifndef KINGVISION_CAMERA_TREE_H
#define KINGVISION_CAMERA_TREE_H

#include <mediasource/cameratree.h>

namespace KingVision {
namespace Internal {

namespace MS = MediaSource;
namespace PE = ProjectExplorer;

class Camera;
class MediaSource;
class CameraTree : public MS::CameraTree
{
    Q_OBJECT

public:
    CameraTree();
    ~CameraTree();

    static CameraTree *instance();

public:
    void setCurrentMediaSource(PE::MediaSource *mediasource);

public:
    ProjectExplorer::FolderNode *rootNode() Q_DECL_OVERRIDE;
    ProjectExplorer::Node *currentNode() Q_DECL_OVERRIDE;

    void nodeChanged(ProjectExplorer::BasicTreeWidget *widget) Q_DECL_OVERRIDE;

private:
    void update(ProjectExplorer::Node *node, MediaSource *currentMediaSource);

private:
    void cameraAboutToBeAdded(Camera *camera);
    void cameraAdded();

    void cameraAboutToBeRemoved(Camera *camera);
    void cameraRemoved();

    void camerasAboutToBeCleared();
    void camerasCleared();

private:
    void cameraAddedImpl(Camera *camera);

private:
    ProjectExplorer::VirtualFolderNode *m_virtualRootNode;  // parent node of toplevel items
    ProjectExplorer::VirtualFolderNode *m_rootNode;         // parent node of all codec devices

private:
    ProjectExplorer::Node *m_currentNode;
    MediaSource *m_currentMediaSource;

private:
    Camera *m_cameraAdded;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_CAMERA_TREE_H
