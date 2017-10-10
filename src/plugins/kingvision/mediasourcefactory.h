#ifndef KINGVISION_MEDIASOURCE_FACTORY_H
#define KINGVISION_MEDIASOURCE_FACTORY_H

#include <mediasource/imediasourcefactory.h>

namespace KingVision {
namespace Internal {

namespace MS = MediaSource;
namespace PE = ProjectExplorer;

class MediaSourceFactory : public MS::IMediaSourceFactory
{
public:
    MediaSourceFactory();

public:
    PE::MediaSource *create(const QVariantMap &map) Q_DECL_OVERRIDE;

public:
    MS::CameraTree *cameraTree() Q_DECL_OVERRIDE;
    MS::DeviceTree *deviceTree() Q_DECL_OVERRIDE;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_MEDIASOURCE_FACTORY_H
