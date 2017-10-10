#ifndef KINGVISION_CAMERA_PLAYER_H
#define KINGVISION_CAMERA_PLAYER_H

#include <StreamPlay/StreamPlay2_G.h>

#include <player/iplayer.h>

namespace KingVision {
namespace Internal {

class Camera;
class CameraPlayerPrivate;
class CameraPlayer : public Player::IPlayer
{
    Q_OBJECT

public:
    explicit CameraPlayer(QObject *parent = 0);
    ~CameraPlayer();

public:
    bool isValid() const;
    QStringList mimeTypes() const;

public:
    ProjectExplorer::MetaData *metaData();

public:
    PlayMode playMode() const;

public:
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;

public slots:
    void play();
    void pause();
    void stop();

protected:
    void aboutToPlayMetaDataImpl(ProjectExplorer::MetaData *metadata) Q_DECL_OVERRIDE;

public:
    Camera *m_camera;

private:
    IStreamPlayPtr m_player;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_CAMERA_PLAYER_H
