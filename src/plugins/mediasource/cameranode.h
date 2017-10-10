#ifndef MEDIASOURCE_CAMERA_NODE_H
#define MEDIASOURCE_CAMERA_NODE_H

#include <projectexplorer/basicnodes.h>
#include <projectexplorer/camera.h>

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class CameraNode : public PE::FolderNode
{
public:
    CameraNode(PE::Camera *camera);

public:
    QString displayName() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;

public:
    PE::Camera *metadata() const Q_DECL_OVERRIDE;

private:
    PE::Camera *m_camera;
};

} // namespace Internal
} // namespace MediaSource

#endif // MEDIASOURCE_CAMERA_NODE_H
