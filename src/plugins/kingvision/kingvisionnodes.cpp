#include <resource/resource.h>

#include "devices.h"
#include "kingvisionnodes.h"

namespace KingVision {
namespace Internal {

CodecNode::CodecNode(Codec *codec)
    : m_codec(codec)
{

}

QString CodecNode::displayName() const
{
    return m_codec->name();
}

QString CodecNode::description() const
{
    return m_codec->description();
}

QIcon CodecNode::icon() const
{
    // TODO: status
    return Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, Resource::Constants::IDX_CODEC);
}

Codec *CodecNode::metadata() const
{
    return m_codec;
}

CameraNode::CameraNode(Camera *camera)
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

Camera *CameraNode::metadata() const
{
    return m_camera;
}

} // namespace Internal
} // namespace KingVision
