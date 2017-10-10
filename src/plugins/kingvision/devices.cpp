#include <utils/communicator/udocaobjecthelper.h>
#include <utils/tmplog.h>

#include "constants.h"
#include "devices.h"

namespace KingVision {
namespace Internal {

Device::Device(const UDOCA::Object &obj)
    : m_object(obj)
{
}

Device::~Device()
{
}

QString Device::safeStringProperty(const char *property) const
{
    return Utils::safeStringProperty(m_object, property);
}

Codec::Codec(const UDOCA::Object &obj)
    : Device(obj)
{
    setId(Core::Id::fromString(safeStringProperty(Constants::KINGVISION_DEVICE_ID)));
}

Codec *Codec::create(const UDOCA::Object &obj)
{
    Codec *codec = new Codec(obj);

    if (!codec->isValid()) {
        delete codec;
        codec = 0;
    }

    return codec;
}

QString Codec::name() const
{
    return safeStringProperty(Constants::KINGVISION_DEVICE_NAME);
}

QString Codec::description() const
{
    return safeStringProperty(Constants::KINGVISION_CODEC_DESCRIPTION);
}

QString Codec::modelId() const
{
    return safeStringProperty(Constants::KINGVISION_CODEC_MODELID);
}


Camera::Camera(const UDOCA::Object &obj)
    : Device(obj)
{
    setId(Core::Id::fromString(safeStringProperty(Constants::KINGVISION_DEVICE_ID)));
}

Camera *Camera::create(const UDOCA::Object &obj)
{
    Camera *camera = new Camera(obj);

    if (!camera->isValid()) {
        delete camera;
        camera = 0;
    }

    return camera;
}

QString Camera::name() const
{
    return safeStringProperty(Constants::KINGVISION_DEVICE_NAME);
}

Core::Id Camera::codecId() const
{
    return Core::Id::fromString(safeStringProperty(Constants::KINGVISION_CAMERA_DEVICEID));
}

qint32 Camera::videoPort() const
{
    return Utils::safeProperty<qint32>(m_object, Constants::KINGVISION_CAMERA_VIDEOPORT, 1);
}


} // namespace Internal
} // namespace KingVision
