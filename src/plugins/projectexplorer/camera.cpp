#include "camera.h"
#include "constants.h"

namespace ProjectExplorer {

Camera::Camera(QObject *parent)
    : MetaData(parent)
{
}

Camera::~Camera()
{
}

QString Camera::mimeType() const
{
    return QLatin1String(Constants::CAMERA_MIMETYPE);
}

} // namespace ProjectExplorer
