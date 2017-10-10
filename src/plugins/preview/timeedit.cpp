#include <QDateTime>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

#include <QTime>

#include "timeedit.h"

namespace Preview {
namespace Internal {

timeEdit::timeEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_isDoubleClicked(false)
    , m_isAbsoluteTime(false)
    , m_playerData(NULL)
{
    setObjectName(QStringLiteral("Preview TimeEdit"));

    setBackGround(Qt::lightGray);
    setFixedWidth(115);
    setAlignment(Qt::AlignRight);
    setReadOnly(true); //Qt::white
    setFrame(false);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &timeEdit::timerTimeOut);

    QFont ft;
    ft.setPixelSize(12);
    setFont(ft);

    connect(this, &timeEdit::editingFinished, this, &timeEdit::timeEdited);
}

timeEdit::~timeEdit()
{

}

void timeEdit::setPlayerData(PlayerData *playerData)
{
    if (!playerData)
        return;

    m_playerData = playerData;

    //setToolTip();
    sToolTip();

    //default show absolute dateTime
    m_isAbsoluteTime = true;
}

void timeEdit::setEditTime(quint64 t)
{
    bool isReadOnly = this->isReadOnly();
    if (!isReadOnly)
        return;

    QString currentTime;

    if (m_isAbsoluteTime)
    {
        currentTime = QDateTime::fromTime_t(t).toString("yyyy-MM-dd hh:mm:ss");
        if (t == 0) // the  end
            currentTime = "0000-00-00 00:00:00";
    }
    else
    {
        currentTime = seconds_to_DHMS(t - m_playerData->getBgnTime());
        if (t == 0)
            currentTime = "00:00:00";
    }

    setText(currentTime);
}

void timeEdit::mousePressEvent(QMouseEvent *event)
{
    m_isDoubleClicked = false;
    m_timer->start(500);
#if 0
    if (m_playerData)
        setReadOnly(false);
#endif
    QLineEdit::mousePressEvent(event);
}

void timeEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    setBackGround(Qt::lightGray);
    m_isDoubleClicked = true;

    if (m_playerData)
        m_isAbsoluteTime = !m_isAbsoluteTime;

    setReadOnly(true);

    QLineEdit::mouseDoubleClickEvent(event);
}

void timeEdit::focusInEvent(QFocusEvent *event)
{
    setReadOnly(false);
    QLineEdit::focusInEvent(event);
}

void timeEdit::focusOutEvent(QFocusEvent *event)
{
    setReadOnly(true);

    QLineEdit::focusOutEvent(event);
}

void timeEdit::timerTimeOut()
{
    if (m_isDoubleClicked)
        return;

    setReadOnly(false);
    setBackGround(Qt::white);
}

void timeEdit::sToolTip()
{
    auto playBack = dynamic_cast<PlayBackData *>(m_playerData);
    if (!playBack)
        return; //TODO; just for PlayBack

    QString bgnTime = QDateTime::fromTime_t(playBack->getBgnTime()).toString("yyyy-MM-dd hh:mm:ss");
    QString endTime = QDateTime::fromTime_t(playBack->getEndTime()).toString("yyyy-MM-dd hh:mm:ss");
    QString timeLength = seconds_to_DHMS(playBack->getTimeRange());

    QString toolTip = tr("begin: ") + bgnTime + "\n" +tr("end: ")
            + endTime + "\n" + tr("length: ") + timeLength;

    setToolTip(toolTip);
}

void timeEdit::timeEdited()
{
    QString editTime = text();
    quint64 offset;
    qDebug() << "editTime text: " << editTime << ": " << m_isAbsoluteTime;

    if (m_isAbsoluteTime)
    {
        quint64 editT = QDateTime::fromString(editTime, "yyyy-MM-dd hh:mm:ss").toTime_t();
        qDebug() << "editT: " << editT << "m_bgnTime: " << m_playerData->getBgnTime();
        offset = editT - m_playerData->getBgnTime();
    }
    else
    {
        QTime eT = QTime::fromString(editTime, "hh:mm:ss");
        offset = eT.hour() *3600 + eT.minute() * 60 + eT.second();
    }

    //if (m_playerData)
    m_playerData->getPlayer()->seek(offset);

    setReadOnly(true);
    setBackGround(Qt::lightGray);

    qDebug() << "bgnTime: " << m_playerData->getBgnTime() << "editTime: " << offset;
}


#if 0
void timeEdit::setAbsoluteTime()
{
    clear();
    setMinimumDateTime(QDateTime::fromTime_t(m_playerData->m_bgnTime));
    setMaximumDateTime(QDateTime::fromTime_t(m_playerData->m_endTime));
    setDisplayFormat("hh:mm:ss");
}

void timeEdit::setRelativeTime()
{
    clear();
    QString cur = "00:00:00";
    setMinimumTime(QTime::fromString(cur, "hh:mm:ss"));
    cur = seconds_to_DHMS(m_playerData->m_timeRange);
    setMaximumTime(QTime::fromString(cur, "hh:mm:ss"));
    setDisplayFormat("hh:mm:ss");
}
#endif

void timeEdit::setBackGround(QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Base, color);//Qt::lightGray);
    palette.setColor(QPalette::Text,Qt::black);
    setPalette(palette);
}

} // namespace Internal
} // namespace Preview

