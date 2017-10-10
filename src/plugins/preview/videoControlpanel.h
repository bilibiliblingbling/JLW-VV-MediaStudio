#ifndef VIDEO_OPERATE_PANLE_H
#define VIDEO_OPERATE_PANLE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "playerwidget.h"
#include "videoControlpanel_p.h"
#include "basicwidget.h"
#include "videogroupoperate.h"
#include "playerdata.h"

namespace Preview {
namespace Internal {

class VideoControlPanel : public BasicWidget
{
    Q_OBJECT

public:
    explicit VideoControlPanel(QWidget *parent = 0);
    ~VideoControlPanel();

    void setPlayer(PlayerData *playerData);

private:
    void panelInit();

signals:
    void currentOperatePanelClicked();
    void videoGroupSynOperate(GroupOperateType t);
    void currentPlayerClosed();

private slots:
    void on_setTimeInBtn_clicked();
    void on_setTimeOutBtn_clicked();
    void on_slider_moved(int timePosition);
    void on_closeBtn_clicked();
    void onUpdateTime(quint64);

public:
    void videoGroupSynCtrl(GroupOperateType t);

private:
    void playerPlay(bool isPlay);
    void playerPause(bool isPause);
    void playerStoped();
    void playerClosed();

private:
    void timeEdited();

public:
    void setTimeIn();
    void setTimeOut();
    void clearTimeInOut();

private:
    Internal::VideoControlPanelPrivate *d;

    Player::IPlayerPtr m_currentPlayer;
    PlayerData *m_playerData;
    PlayBackData *m_playBackData;
    LivePlayerData *m_livePlayData;

    bool m_isSeek;
};

} // namespace Internal
} // namespace Preview

#endif // VIDEO_OPERATE_PANLE_H
