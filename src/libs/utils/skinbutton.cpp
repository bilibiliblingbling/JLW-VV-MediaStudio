#include <QPainter>
#include <QMenu>

#include <resource/resource.h>

#include "skinbutton.h"
#include "tmplog.h"


namespace Utils {

/*!
 * \brief The SkinButton class
 * customized button with full style support, dynamically draw button pixmap
 */

SkinButton::SkinButton(qint32 type, QWidget *parent)
    : QPushButton(parent)
    , m_type(type)
    , m_index(0)
    , m_state(ButtonNormal)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(setIndex(bool)));
}

void SkinButton::setIndex(bool checked)
{
    TM_DBG << checked;
    m_index = checked? 1: 0;
}

void SkinButton::setIndex(qint8 idx)
{
    TM_DBG << idx;
    m_index = idx;
}

QSize SkinButton::sizeHint() const
{
    return getStatePixmap().size();
}

void SkinButton::enterEvent(QEvent *e)
{
    TM_MARK;
    m_state = ButtonHovered;
    QPushButton::enterEvent(e);
}

void SkinButton::leaveEvent(QEvent *e)
{
    TM_MARK;
    m_state = ButtonNormal;
    QPushButton::leaveEvent(e);
}

void SkinButton::mousePressEvent(QMouseEvent *e)
{
    TM_MARK;
    m_state = ButtonPressed;
    QPushButton::mousePressEvent(e);
}

void SkinButton::mouseReleaseEvent(QMouseEvent *e)
{
    TM_MARK;
    m_state = ButtonHovered;
    QPushButton::mouseReleaseEvent(e);
}

void SkinButton::paintEvent(QPaintEvent *)
{
    QPainter ptr(this);
    TM_DBG("m_index: %d, m_state: %d", m_index, m_state);
    ptr.drawPixmap(0, 0, width(), height(), getStatePixmap());
}

QPixmap SkinButton::getStatePixmap() const
{
    return Resource::PixmapManager::inst().getPixmap((Resource::IndexPixmapType)m_type, m_index*4 + m_state);
}

} // namespace Utils
