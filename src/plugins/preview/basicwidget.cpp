#include "basicwidget.h"

namespace Preview {
namespace Internal {

BasicWidget::BasicWidget(QWidget *parent , Qt::WindowFlags f)
    : QWidget(parent, f)
{
    setObjectName(QStringLiteral("Basic Widget"));
    setFocusPolicy(Qt::StrongFocus);
}

void BasicWidget::mousePressEvent(QMouseEvent *ev)
{
    QWidget::mousePressEvent(ev);
}

void BasicWidget::setWidgetBackground(QColor color)
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}


} // namespace Internal
} // namespace Preview
