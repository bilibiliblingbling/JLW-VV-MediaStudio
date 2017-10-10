#ifndef PLAYER_WIDGET_H
#define PLAYER_WIDGET_H

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QWidget>
#include <player/iscreen.h>
#include <clearvision/segmentplayer.h>


namespace Monitor {
namespace Internal {

class PlayerWidget : public QWidget, public Player::IScreen
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();

public:
    HWND getWinId()const;

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    QPaintEngine *paintEngine() const
    {
        return NULL;
    }

private:
    void setBackground(QColor color);

private:
    Player::IPlayerPtr m_player;

};

} // namespace Internal
} // namespace Monitor

#endif // PLAYER_WIDGET_H
