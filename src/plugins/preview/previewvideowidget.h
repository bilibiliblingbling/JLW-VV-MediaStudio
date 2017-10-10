#ifndef PREVIEW_VIDEO_WIDGET_H
#define PREVIEW_VIDEO_WIDGET_H

#include <QTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QSplitter>
#include <QSharedPointer>

#include "videoControlpanel.h"
#include "playerwidget.h"
#include "playerdata.h"
#include "videogroupoperate.h"

namespace Preview {
namespace Internal {

class PreviewVideoWidget : public QSplitter
{
    Q_OBJECT
public:
    explicit PreviewVideoWidget(QWidget *parent = 0);
    ~PreviewVideoWidget();

    void setVideoFocus(bool bFocus);
    void setVideoInGroup(QColor color);
#if 1
private slots:
    void loadVideoControlPanel(const Player::IPlayerPtr &player);
#endif
signals:
    void widgetClicked(PreviewVideoWidget *currentWidget);
    void videoGroupSynOperateSignal(PreviewVideoWidget *currentWidget, GroupOperateType t);

protected:
    //void dragEnterEvent(QDragEnterEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *ev);

public:
    void videoGroupSynCtrl(GroupOperateType t);
    PlayerData *getCurrentPlayerData() const;

private:
    void playerClosed();
    void setBackground(QColor color);

public:
    void setTimeIn();
    void setTimeOut();
    void clearTimeInOut();

public:
    quint8 m_groupIndex;

private:
    VideoControlPanel *m_videoControlPanel;
    QTimer *m_timer;
    PlayerWidget *m_videoWidget;
    BasicWidget *m_videoInWhichGroup;

    bool m_isPlayerCreated;
    Player::IPlayerPtr m_currentPlayer;
    PlayerData *m_playerData;
};

} // namespace Internal
} // namespace Preview


#endif // PREVIEW_VIDEO_WIDGET_H
