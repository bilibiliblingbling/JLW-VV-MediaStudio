#include "fileplayviewwidget.h"

namespace Monitor {
namespace Internal {

FilePlayViewWidget::FilePlayViewWidget(QWidget *parent)
    : QWidget(parent)
{
    setBackground(Qt::lightGray);
}

FilePlayViewWidget::~FilePlayViewWidget()
{

}

void FilePlayViewWidget::setBackground(QColor color)
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

} // namespace Internal
} // namespace Monitor

