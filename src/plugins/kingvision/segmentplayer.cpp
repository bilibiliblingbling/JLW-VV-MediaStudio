#include <utils/tmplog.h>

#include "segmentplayer.h"
#include "segmentplayer_p.h"

namespace KingVision {
namespace Internal {

SegmentPlayer::SegmentPlayer(QObject *parent)
    : Player::IPlayer(parent)
    , d(new SegmentPlayerPrivate(this))
{
    setObjectName(QStringLiteral("KingVision.SegmentPlayer"));
}

SegmentPlayer::~SegmentPlayer()
{
#if 0
    delete d;
    d = 0;
#endif
}

bool SegmentPlayer::isValid() const
{
    return (d->m_player);
}

Player::IPlayer::PlayMode SegmentPlayer::playMode() const
{
    return PlaybackMode;
}

bool SegmentPlayer::isPlaying() const
{
    return (d->m_player && d->m_player->IsPlaying());
}

bool SegmentPlayer::isPaused() const
{
    return (d->m_player && d->m_player->IsPaused());
}

bool SegmentPlayer::isStopped() const
{
    return (d->m_player && d->m_player->IsStopped());
}

void SegmentPlayer::play()
{
    // TOOD
    d->play();
}

void SegmentPlayer::pause()
{
    // TOOD
}

void SegmentPlayer::stop()
{
    // TODO
}

bool SegmentPlayer::isCyclic() const
{
    return d->m_cyclic;
}

void SegmentPlayer::setCyclic(bool b)
{
    if (d->m_cyclic != b) {
        d->m_cyclic = b;
    }

    if (d->m_cyclic) {
        // do something
    }
}

double SegmentPlayer::speedRate() const
{
    return d->m_speedRate;
}

void SegmentPlayer::setSpeedRate(double rate)
{
    d->m_speedRate = rate;

    // TODO:
    if (d->m_player)
        d->m_player->SetRate(d->m_speedRate);
}

void SegmentPlayer::stepFrame(qint32 frames)
{
    // TODO
}


////////////////////////////////////////////////////////////////////////////////


SegmentPlayerPrivate::SegmentPlayerPrivate(QObject *parent)
    : QObject(parent)
    , m_cyclic(false)
    , m_speedRate(0.)
    , m_screen(0)
{
    m_player = ::CreateStreamPlay();
}

SegmentPlayerPrivate::~SegmentPlayerPrivate()
{
}

void SegmentPlayerPrivate::setScreen(QWidget *screen)
{
    if (m_screen == screen)
        return;

    TM_DBG("about to change screen: (%p -> %p)", m_screen, screen);

    if (m_screen)
        m_screen->disconnect(this);

    // should not change screen
    m_screen = screen;

    connect(m_screen, &QWidget::destroyed,
            this, &SegmentPlayerPrivate::screenDestroied);
}

void SegmentPlayerPrivate::screenDestroied(QObject *screen)
{
    // do something
    TM_DBG("screen(%p) destroied", screen);
}

void SegmentPlayerPrivate::play()
{
    if (!m_screen) {
        TM_DBG << "no screen set, no need to play";
        return;
    }

    // open stream
}

HRESULT SegmentPlayerPrivate::OnBufferAvailable(DWORD dwStreamType)
{
    // TODO
}

HRESULT SegmentPlayerPrivate::OnStreamFlag(const STREAM_INFO *pStreamInfo, LONG lHandled, LONG *plStatus)
{

}

HRESULT SegmentPlayerPrivate::OnAudioDecode(const AUDIO_FRAME *pFrame)
{

}

HRESULT SegmentPlayerPrivate::OnVideoDecode(const VIDEO_FRAME *pFrame)
{

}

HRESULT SegmentPlayerPrivate::BeforeVideoRender(IVideoFrame *pFrame)
{

}

} // namespace Internal
} // namespace KingVision
