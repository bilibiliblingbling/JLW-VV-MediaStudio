#include <QAction>
#include <QBoxLayout>
#include <QPointer>
#include <QResizeEvent>

#include "pane.h"

namespace Core {
namespace Internal {

class PanePrivate
{
public:
    PanePrivate(Qt::Orientation orientation = Qt::Horizontal)
        : m_orientation(orientation)
        , m_shown(true)
        , m_suppressed(false)
        , m_size(0)
    {}

public:
    Qt::Orientation m_orientation;

    bool m_shown;
    bool m_suppressed;
    qint32 m_size;

    QPointer<QWidget> m_widget;
};

} // namespace Internal


Pane::Pane(QWidget *parent)
    : QWidget(parent)
    , d(new Internal::PanePrivate())
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    setLayout(layout);
}

Pane::~Pane()
{
    clearWidget();

    delete d;
    d = 0;
}

void Pane::setWidget(QWidget *widget)
{
    if (widget == d->m_widget) {
        setShown(true);
        return;
    }

    clearWidget();

    d->m_widget = widget;
    if (d->m_widget) {
        setSizePolicy(d->m_widget->sizePolicy());
        resize(d->m_widget->size());

        d->m_widget->setParent(this);
        layout()->addWidget(d->m_widget);
        setFocusProxy(d->m_widget);
        setShown(true);
    }
}

bool Pane::validateWidget() const
{
    return d->m_widget;
}

qint32 Pane::storedSize() const
{
    return d->m_size;
}

void Pane::restoreSize(qint32 size)
{
    d->m_size = size;
}

bool Pane::isShown() const
{
    return d->m_shown;
}

void Pane::setShown(bool b)
{
    if (d->m_widget) {
        d->m_widget->setVisible(b);
    }
}

bool Pane::isSuppressed() const
{
    return d->m_suppressed;
}

void Pane::setSuppressed(bool b)
{
    if (d->m_suppressed == b)
        return;

    d->m_suppressed = b;
}

Qt::Orientation Pane::orientation() const
{
    return d->m_orientation;
}

void Pane::setOrientation(Qt::Orientation orientation)
{
    d->m_orientation = orientation;
}

QSize Pane::sizeHint() const
{
    if (d->m_widget)
        return d->m_widget->sizeHint();

    return QWidget::sizeHint();
}

void Pane::resizeEvent(QResizeEvent *re)
{
    if (d->m_size) {
        if (d->m_orientation == Qt::Horizontal && re->size().width())
            d->m_size = re->size().width();
        if (d->m_orientation == Qt::Vertical && re->size().height())
            d->m_size = re->size().height();
    }

    QWidget::resizeEvent(re);
}

void Pane::clearWidget()
{
    if (d->m_widget) {
        setShown(false);

        d->m_widget->setParent(0);
        d->m_widget = 0;
    }
}

} // namespace Core
