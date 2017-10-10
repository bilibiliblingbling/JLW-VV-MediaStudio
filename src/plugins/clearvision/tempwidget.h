#ifndef TEMPWIDGET_H
#define TEMPWIDGET_H
#include "segmentplayer.h"

#include <QWidget>
#include <QDragEnterEvent>
#include <QPushButton>

#include "player/iscreen.h"
class QDragEnterEvent;

class tempWidget : public QWidget, public Player::IScreen
{
    Q_OBJECT
public:
    explicit tempWidget(QWidget *parent = 0);

    // IScreen interface
public:
    HWND getWinId() const Q_DECL_OVERRIDE;

    void showVideo() Q_DECL_OVERRIDE;
    void showBackGround() Q_DECL_OVERRIDE;
    void showMessage(const QString &msg) Q_DECL_OVERRIDE;


protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
public:
    quint32 m_bgnTime;
    quint32 m_endTime;

public:
    ClearVision::Internal::SegmentPlayer *segPlay;

};

#endif // TEMPWIDGET_H
