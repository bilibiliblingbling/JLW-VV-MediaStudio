#include "sequenceplayviewwidget.h"

namespace Monitor {
namespace Internal {

SequencePlayViewWidget::SequencePlayViewWidget(QWidget *parent)
    : QWidget(parent)
{
    setBackground(Qt::lightGray);
}

SequencePlayViewWidget::~SequencePlayViewWidget()
{

}

void SequencePlayViewWidget::setBackground(QColor color)
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

} // namespace Internal
} // namespace Monitor
