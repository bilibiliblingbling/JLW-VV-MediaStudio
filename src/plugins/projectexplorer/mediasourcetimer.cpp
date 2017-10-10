#include <QTimer>

#include "mediasourcetimer.h"

namespace ProjectExplorer {
namespace Internal {

const int MEDIASOURCE_CHECKSTATUS_INTERVAL  = 500;

static MediaSourceTimer *s_timer = 0;

MediaSourceTimer::MediaSourceTimer()
    : m_timer(new QTimer(this))
{
    setObjectName(QLatin1String("ProjectExplorer.MediaSourceTimer"));

    m_timer->setInterval(MEDIASOURCE_CHECKSTATUS_INTERVAL);
    connect(m_timer, &QTimer::timeout, this, &MediaSourceTimer::checkStatus);
    s_timer = this;
}

MediaSourceTimer::~MediaSourceTimer()
{
    s_timer = 0;
}

MediaSourceTimer *MediaSourceTimer::instance()
{
    return s_timer;
}

void MediaSourceTimer::extensionsInitialized()
{
    s_timer->m_timer->start();
}

} // namespace Internal
} // namespace ProjectExplorer
