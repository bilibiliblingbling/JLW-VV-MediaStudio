#ifndef KINGVISION_SEGMENT_PLAYER_H
#define KINGVISION_SEGMENT_PLAYER_H

#include <player/iplayer.h>

namespace KingVision {
namespace Internal {

class SegmentPlayer;
typedef QSharedPointer<SegmentPlayer> SegmentPlayerPtr;

class SegmentPlayerPrivate;
class SegmentPlayer : public Player::IPlayer
{
    Q_OBJECT

public:
    explicit SegmentPlayer(QObject *parent = 0);
    ~SegmentPlayer();

public:
    bool isValid() const Q_DECL_OVERRIDE;

public:
//    void playMetaData();

public:
    PlayMode playMode() const Q_DECL_OVERRIDE;

public:
    bool isPlaying() const Q_DECL_OVERRIDE;
    bool isPaused() const Q_DECL_OVERRIDE;
    bool isStopped() const Q_DECL_OVERRIDE;

    Q_SLOT void play() Q_DECL_OVERRIDE;
    Q_SLOT void pause() Q_DECL_OVERRIDE;
    Q_SLOT void stop() Q_DECL_OVERRIDE;

    ////////////////////////////////////////////////////////////////////////////
public:
    bool isCyclic() const Q_DECL_OVERRIDE;

    Q_SLOT virtual void setCyclic(bool b);

public:
    double speedRate() const Q_DECL_OVERRIDE;
    Q_SLOT void setSpeedRate(double rate) Q_DECL_OVERRIDE;

    Q_SLOT void stepFrame(qint32 frames) Q_DECL_OVERRIDE;

private:
    SegmentPlayerPrivate *d;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_SEGMENT_PLAYER_H
