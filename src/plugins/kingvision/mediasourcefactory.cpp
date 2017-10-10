#include <QCoreApplication>

#include "cameratree.h"
#include "constants.h"
#include "devicetree.h"
#include "mediasource.h"
#include "mediasourcefactory.h"

namespace KingVision {
namespace Internal {

MediaSourceFactory::MediaSourceFactory()
    : MS::IMediaSourceFactory(Constants::MEDIASOURCE_CLASSID, QCoreApplication::translate("KingVision", Constants::MEDIASOURCE_CLASSNAME))
{
}

PE::MediaSource *MediaSourceFactory::create(const QVariantMap &map)
{
    MediaSource *mediasource = new MediaSource();
    if (!mediasource->fromMap(map)) {
        delete mediasource;
        mediasource = 0;
    }
    return mediasource;
}

MS::CameraTree *MediaSourceFactory::cameraTree()
{
    return CameraTree::instance();
}

MS::DeviceTree *MediaSourceFactory::deviceTree()
{
    return DeviceTree::instance();
}

} // namespace Internal
} // namespace KingVision
