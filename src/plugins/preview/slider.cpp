#include "constants.h"
#include "slider.h"

#include <QDebug>

namespace Preview {
namespace Internal {

Slider::Slider(QWidget *parent)
    : QSlider(parent)
    , m_isRestore(false)
    , m_timeInBtnClicked(false)
    , m_timeOutBtnClicked(false)
    , m_isClickTimeOutFirst(false)
    , m_timeInPointX(0)
    , m_timeOutPointX(0)
{
    setOrientation(Qt::Horizontal);
    setCursor(Qt::PointingHandCursor);
    setFixedHeight(5);
    setStyleSheet(Constants::SLIDERSTYLESHEET);
}

void Slider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int position = 0;
        if (orientation() == Qt::Vertical)
        {
            position = minimum() + ((maximum()-minimum()) * (height()-event->y())) / height();
            setValue(position);
        }
        else
        {
            position = minimum() + ((maximum()-minimum()) * event->x()) / width();
            setValue(position) ;
        }
        m_xPoint = event->x();
        emit sliderMoved(position);
        event->setAccepted(false);
    }
    emit sliderClicked();
    QSlider::mousePressEvent(event);
}

void Slider::paintEvent(QPaintEvent *event)
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    opt.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
    if (tickPosition() != NoTicks) {
        opt.subControls |= QStyle::SC_SliderTickmarks;
    }

    QRect groove_rect = style()->subControlRect(
                QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

    QSlider::paintEvent(event);

    if (m_timeInBtnClicked)
    {
        m_timeInPointX = m_xPoint;
        m_timeInBtnClicked = false;
        m_isRestore = false;
        //m_timeOutPointX = 0;
    }
    if (m_timeOutBtnClicked)
    {
        m_timeOutPointX = m_xPoint;
        m_timeOutBtnClicked = false;
        m_isRestore = false;
    }

    if (m_timeOutPointX || m_timeInPointX)
    {
        if (m_timeInPointX && m_isClickTimeOutFirst == false)
            m_needPaintWidth = m_timeOutPointX == 0 ? groove_rect.width() : m_timeOutPointX - m_timeInPointX;
        else if (m_timeOutPointX && m_isClickTimeOutFirst == false && !m_timeInPointX)
            m_isClickTimeOutFirst = true;

        //click TimeOut button firstly, paint slider 0 to TimeOut's X,then if clicked TimeIn button,
        // need abandon the TimeOut clicked, only paint the TimeIn's X to the end of slider
        if (m_timeOutPointX && m_isClickTimeOutFirst == true)
        {
            if (m_timeInPointX)
            {
                //m_timeOutPointX = 0;
                m_needPaintWidth = groove_rect.width();
            }
            else
                m_needPaintWidth = m_timeOutPointX;
        }
    }
    else {
        m_isRestore = true;
    }
#if 0
    if (m_timeInPointX)
        width = m_timeOutPointX == 0 ? groove_rect.width() : m_timeOutPointX - m_timeInPointX;
    else
        width = m_timeOutPointX == 0 ? m_isRestore = true : m_timeOutPointX;
#endif

    if (m_isRestore) // slider keep it's default styleSheet
    {
        m_timeInPointX = 0;
        m_timeOutPointX = 0;
        return;
    }

    QRect rect(m_timeInPointX, groove_rect.top(), m_needPaintWidth, groove_rect.height());
    QPainter painter(this);
    painter.fillRect(rect, QBrush(Qt::red));
}

void Slider::sliderRestoreDefault()
{
    m_isRestore = true;
    m_isClickTimeOutFirst = false;
    this->update();
}

void Slider::sliderSetTimeIn(quint16 pos)
{
    m_xPoint = pos;
    m_timeInBtnClicked = true;
    this->update();
}

void Slider::sliderSetTimeOut(quint16 pos)
{
    m_xPoint = pos;
    m_timeOutBtnClicked = true;
    this->update();
}

void Slider::sliderSetTimeIn()
{
    m_timeInBtnClicked = true;
    this->update();
}

void Slider::sliderSetTimeOut()
{
    m_timeOutBtnClicked = true;
    this->update();
}

} // namespace Internal
} // namespace Preview
