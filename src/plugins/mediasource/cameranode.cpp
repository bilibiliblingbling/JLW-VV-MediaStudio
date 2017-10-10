#include <resource/resource.h>

#include "cameranode.h"

namespace MediaSource {
namespace Internal {

CameraNode::CameraNode(PE::Camera *camera)
    : m_camera(camera)
{
}

QString CameraNode::displayName() const
{
    return m_camera->name();
}

QIcon CameraNode::icon() const
{
    return Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, Resource::Constants::IDX_CAMERA);
}

ProjectExplorer::Camera *CameraNode::metadata() const
{
    return m_camera;
}

} // namespace Internal
} // namespace MediaSource
