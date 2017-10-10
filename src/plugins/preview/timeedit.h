#ifndef TIME_EDIT_H
#define TIME_EDIT_H

#include <QLineEdit>
#include <QMouseEvent>
#include <QTimer>

#include "playerdata.h"

namespace Preview {
namespace Internal {

QString seconds_to_DHMS(quint64 duration);

class timeEdit : public QLineEdit
{
public:
    explicit timeEdit(QWidget *parent = 0);
    ~timeEdit();

public:
    void setPlayerData(PlayerData *playerData);
    void setEditTime(quint64 t);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

private slots:
    void timerTimeOut();

private:
    void sToolTip();
    void timeEdited();
    void setBackGround(QColor color);

private:
    bool m_isDoubleClicked;
    QTimer *m_timer;
    bool m_isAbsoluteTime;
    PlayerData *m_playerData;
};


} // namespace Internal
} // namespace Preview

#endif // TIME_EDIT_H
