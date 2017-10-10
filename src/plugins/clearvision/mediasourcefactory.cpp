#include <QCoreApplication>

#include "cameratree.h"
#include "constants.h"
#include "mediasource.h"
#include "mediasourcefactory.h"

namespace ClearVision {
namespace Internal {

MediaSourceFactory::MediaSourceFactory()
    : MS::IMediaSourceFactory(Constants::MEDIASOURCE_CLASSID, QCoreApplication::translate("ClearVision", Constants::TR_MEDIASOURCE_CLASSNAME))
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
    return CameraTree::instance();
}

bool MediaSourceFactory::queryRecordType() const
{
    return true;
}

} // namespace Internal
} // namespace ClearVision
