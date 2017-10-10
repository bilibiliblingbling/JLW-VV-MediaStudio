#ifndef PLAYER_WIDGET_H
#define PLAYER_WIDGET_H

#include <QDragEnterEvent>

#include "basicwidget.h"
#include "clearvision/segmentplayer.h"
#include "player/iscreen.h"

//class ClearVision::Internal::SegmentPlayer;
namespace Preview {
namespace Internal {

class PlayerWidget : public BasicWidget, public Player::IScreen
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();

    // IScreen interface
public:
    HWND getWinId() const Q_DECL_OVERRIDE;

    void showVideo() Q_DECL_OVERRIDE;
    void showBackGround() Q_DECL_OVERRIDE;
    void showMessage(const QString &msg) Q_DECL_OVERRIDE;

signals:
    void currentWidgetAcceptDropEvent();
    void createPlayerSuccessed(Player::IPlayerPtr &);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    //void resizeEvent(QResizeEvent *);
    QPaintEngine *paintEngine() const
    {
        return NULL;
    }

private:
    //Player::IPlayerPtr m_player;

};

} // namespace Internal
} // namespace Preview

#endif // PLAYER_WIDGET_H
