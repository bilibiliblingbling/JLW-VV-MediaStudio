#ifndef IPLAYER_H
#define IPLAYER_H

#include <QObject>
#include <QSharedPointer>

#include <projectexplorer/metadata.h>

#include "player_global.h"

namespace Player {

class IPlayer;
class IScreen;
typedef QSharedPointer<IPlayer> IPlayerPtr;

class PLAYER_EXPORT IPlayer : public QObject
{
    Q_OBJECT

public:
    enum PlayMode {
          InvalidMode
        , LiveMode
        , PlaybackMode
    };

public:
    explicit IPlayer(QObject *parent = 0);
    virtual ~IPlayer();

public:
    virtual bool isValid() const = 0;
    virtual QStringList mimeTypes() const = 0;

public:
    virtual ProjectExplorer::MetaData *metaData();
    Q_SLOT void aboutToPlayMetaData(ProjectExplorer::MetaData *metadata);

public:
    void setScreen(IScreen *screen);
    IScreen *getScreen();

public:
    virtual PlayMode playMode() const = 0;

public:
    virtual bool isPlaying() const = 0;
    virtual bool isPaused() const = 0;
    virtual bool isStopped() const = 0;

    Q_SLOT virtual void play() = 0;
    Q_SLOT virtual void pause() = 0;
    Q_SLOT virtual void stop() = 0;

public:
    Q_SIGNAL void stateChanged();

    ////////////////////////////////////////////////////////////////////////////
    // should be valid in live mode
public:
    /*!
     * \brief canRecord
     * \return 是否可以进行录像
     */
    virtual bool canRecord() const;
    virtual bool isRecording() const;
    Q_SLOT virtual void setRecording(bool b);


    ////////////////////////////////////////////////////////////////////////////
    // should be valid in playback mode
public:
    /*!
     * \brief isCyclic
     * \return 是否循环播放
     */
    virtual bool isCyclic() const;
    Q_SLOT virtual void setCyclic(bool b);

public:
    /*!
     * \breif elapse
     * \param t
     *  \todo \sa seek
     */
    Q_SIGNAL void elapse(quint64 t);

    /*!
     * \brief currentTime
     * \return 当前播放时间
     */
    virtual quint64 currentTime() const;

    /*!
     * \brief beginTime
     * \return 起始时间
     * \todo 名字也许会改
     */
    virtual quint64 beginTime() const;

    /*!
     * \brief endTime
     * \return 结束时间
     * \todo 名字也许会改
     */
    virtual quint64 endTime() const;


    /*!
     * \brief reachEnd
     *  播放结束
     */
    Q_SIGNAL void reachEnd();

    /*!
     * \brief seek
     * \param t, 单位 s
     *  跳到指定时间 \p t
     */
    Q_SLOT virtual void seek(quint64 t);

    /*!
     * \brief skip
     * \param interval, 间隔, 跳跃幅度
     *  快速浏览, 跳跃播放
     *      > 0, 快进
     *      = 0, 取消快速浏览, 即正常播放
     *      < 0, 快退
     */
    Q_SLOT virtual void skip(qint32 interval);

public:
    /*!
     * \brief speedRate
     * \return 当前播放速度
     */
    virtual double speedRate() const;
    Q_SLOT virtual void setSpeedRate(double rate);

public:
    /*!
     * \brief stepFrame
     *  逐帧, 单帧, 下 1 帧
     */
    Q_SLOT void stepFrame();

    /*!
     * \brief stepFrame
     * \p no
     *  其后第 \p no 帧
     */
    Q_SLOT virtual void stepFrame(qint32 no);

protected:
    virtual void aboutToPlayMetaDataImpl(ProjectExplorer::MetaData *metadata) = 0;

private:
    IScreen *m_screen;
};

} // namespace Player

#endif // IPLAYER_H
