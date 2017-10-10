#include <QBoxLayout>
#include <QLabel>

#include "basicscreen.h"
#include "basicscreen_p.h"

namespace Player{

BasicScreen::BasicScreen(QWidget *parent)
    : QWidget(parent)
    , d(new Internal::BasicScreenPrivate)
{
    d->m_stack = new QStackedWidget(this);

    d->m_background = new Internal::BackGroundWidget();
    d->m_video = new Internal::VideoWidget();
    d->m_message = new Internal::MessageWidget();

    d->m_stack->addWidget(d->m_background);
    d->m_stack->addWidget(d->m_video);
    d->m_stack->addWidget(d->m_message);

    d->m_stack->setCurrentWidget(d->m_background);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->setContentsMargins(0, 0, 0, 0);
    mainlayout->setSpacing(0);
    mainlayout->addWidget(d->m_stack);
}

HWND BasicScreen::getWinId() const
{
    return d->m_video->getRealPlayWindowId();
}

void BasicScreen::showBackGround()
{
    d->m_stack->setCurrentWidget(d->m_background);
}

void BasicScreen::showMessage(const QString &msg)
{
    d->m_message->showMessage(msg);
    d->m_stack->setCurrentWidget(d->m_message);
}

void BasicScreen::showVideo()
{
    d->m_stack->setCurrentWidget(d->m_video);
}

////////////////////////////////////////////////////////////////////////////////

namespace Internal {

MessageWidget::MessageWidget()
    : m_message(0)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    m_message = new QLabel(this);
    pal = m_message->palette();
    pal.setColor(m_message->foregroundRole(), Qt::white);
    m_message->setPalette(pal);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_message);
    layout->setAlignment(m_message, Qt::AlignCenter);
}

void MessageWidget::showMessage(const QString &msg)
{
    m_message->setText(msg);
}

////////////////////////////////////////////////////////////////////////////////

BackGroundWidget::BackGroundWidget()
{
    // TODO: 背景需要修改
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////

VideoWidget::VideoWidget()
    : m_realvideo(0)
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(Pal);

    m_realvideo = new RealVideo(this);
    m_realvideo->setAttribute(Qt::WA_PaintOnScreen);

    QLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_realvideo);
}

HWND VideoWidget::getRealPlayWindowId() const
{
    return (HWND)m_realvideo->winId();
}

} // NameSpace Internal
} // NameSpace Player
