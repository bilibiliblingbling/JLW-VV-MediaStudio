#include <QTime>

#include <resource/resource.h>

#include "mediasourcenode.h"

namespace MediaSource {
namespace Internal {

MediaSourceNode::MediaSourceNode(PE::MediaSource *source)
    : m_mediasource(source)
{
}

QString MediaSourceNode::displayName() const
{
    return m_mediasource->name();
}

bool MediaSourceNode::enabled() const
{
    return m_mediasource->enabled();
}

QIcon MediaSourceNode::icon() const
{
    int idx = Resource::Constants::IDX_DISABLED;
    switch (m_mediasource->status()) {
    case PE::MediaSource::StatusDisabled:
        idx = Resource::Constants::IDX_DISABLED;
        break;
    case PE::MediaSource::StatusLoggingIn:
        idx = Resource::Constants::IDX_LOGGING_IN_RIGHT
              + QTime::currentTime().msec()/500;
        break;
    case PE::MediaSource::StatusOnline:
        idx = Resource::Constants::IDX_LOGGED_IN;
        break;
    case PE::MediaSource::StatusOffline:
        idx = Resource::Constants::IDX_OFFLINE;
        break;
    }

    return QIcon(Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, idx));
}

PE::MediaSource *MediaSourceNode::metadata() const
{
    return m_mediasource;
}

} // namespace Internal
} // namespace MediaSource
