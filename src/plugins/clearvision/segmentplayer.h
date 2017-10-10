#ifndef CLEARVISION_SEGMENT_PLAYER_H
#define CLEARVISION_SEGMENT_PLAYER_H

#include <QMutex>

#include <player/iplayer.h>

namespace ClearVision {
namespace Internal {

class SegmentPlayerPrivate;
typedef QSharedPointer<ProjectExplorer::MetaData> MetaDataPtr;

class SegmentPlayer : public Player::IPlayer
{
    Q_OBJECT

public:
    explicit SegmentPlayer(QObject *parent = 0);
    ~SegmentPlayer();

public:
    bool isValid() const;

    QStringList mimeTypes() const;

public:
    PlayMode playMode() const;

public:
    bool isOpened() const;
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;

    void play();
    void pause();
    void stop();
    void close();
    void seek(quint64 offset);
    void skip(quint32 intervalSec);
    quint64 beginTime() const;
    quint64 endTime() const;

public:
    bool isCyclic() const;
    Q_SLOT void setCyclic(bool b);

public:
    double speedRate() const;
    Q_SLOT void setSpeedRate(double rate);

public:
    Q_SLOT void stepFrames(qint32 frames);

public:
    quint64 currentTime()const;

    void playRecord(ProjectExplorer::MetaData *metadata);

protected:
    void timerEvent(QTimerEvent *e);

protected:
    void aboutToPlayMetaDataImpl(ProjectExplorer::MetaData *metadata);

protected:
    //void updatePlayButton(bool bPlaying);

private:
    SegmentPlayerPrivate *d;
    ProjectExplorer::MetaData *m_metadata;

};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_SEGMENT_PLAYER_H
