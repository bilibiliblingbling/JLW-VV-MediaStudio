#include "cameraplayer.h"
#include "constants.h"
#include "devices.h"

namespace KingVision {
namespace Internal {

CameraPlayer::CameraPlayer(QObject *parent)
    : Player::IPlayer(parent)
    , m_camera(0)
{
    m_player = ::CreateStreamPlay();
}

CameraPlayer::~CameraPlayer()
{
}

bool CameraPlayer::isValid() const
{
    return m_player;
}

QStringList CameraPlayer::mimeTypes() const
{
    return (QStringList() << QLatin1String(Constants::KINGVISION_CAMERA_MIMETYPE));
}

ProjectExplorer::MetaData *CameraPlayer::metaData()
{
    return 0;
}

Player::IPlayer::PlayMode CameraPlayer::playMode() const
{
    return LiveMode;
}

bool CameraPlayer::isPlaying() const
{
    // TODO
    return (m_player && m_player->IsPlaying());
}

bool CameraPlayer::isPaused() const
{
    // TODO
    return (m_player && m_player->IsPaused());
}

bool CameraPlayer::isStopped() const
{
    // TODO
    return (m_player && m_player->IsStopped());
}

void CameraPlayer::play()
{
    // TODO

}

void CameraPlayer::pause()
{
    // TODO
}

void CameraPlayer::stop()
{
    // TODO
}

void CameraPlayer::aboutToPlayMetaDataImpl(ProjectExplorer::MetaData *metadata)
{
    Camera *camera = qobject_cast<Camera *>(metadata);
    if (!camera || !camera->isValid())
        return;

    // TOOD: open stream
    // fallback to DeviceLayer? that's not good!

    // TODO: play stream
    // camera video propertyies,
    // video algorithm, audio algorithm

    // let's play playback-segment first
}

} // namespace Internal
} // namespace KingVision
