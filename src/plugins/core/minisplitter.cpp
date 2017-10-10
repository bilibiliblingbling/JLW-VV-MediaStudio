#include <QPaintEvent>
#include <QPainter>
#include <QSplitterHandle>

#include <utils/stylehelper.h>

#include "minisplitter.h"

namespace Core {
namespace Internal {

class MiniSplitterHandle : public QSplitterHandle
{
public:
    MiniSplitterHandle(Qt::Orientation orientation, QSplitter *parent, bool lightColored = false)
            : QSplitterHandle(orientation, parent),
              m_lightColored(lightColored)
    {
        setMask(QRegion(contentsRect()));
        setAttribute(Qt::WA_MouseNoMask, true);
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
        if (orientation() == Qt::Horizontal)
            setContentsMargins(2, 0, 2, 0);
        else
            setContentsMargins(0, 2, 0, 2);
        setMask(QRegion(contentsRect()));
        QSplitterHandle::resizeEvent(event);
    }

    void paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.fillRect(event->rect(), Utils::StyleHelper::borderColor(m_lightColored));
    }

private:
    bool m_lightColored;
};

} // namespace Internal


MiniSplitter::MiniSplitter(QWidget *parent, SplitterStyle style)
    : QSplitter(parent)
    , m_style(style)
{
    setHandleWidth(1);
    setChildrenCollapsible(false);
    setProperty("minisplitter", true);
}

MiniSplitter::MiniSplitter(Qt::Orientation orientation, SplitterStyle style)
    : QSplitter(orientation)
    , m_style(style)
{
    setHandleWidth(1);
    setChildrenCollapsible(false);
    setProperty("minisplitter", true);
}

QSplitterHandle *MiniSplitter::createHandle()
{
    return new Internal::MiniSplitterHandle(orientation(), this, m_style == Light);
}


} // namespace Core
