#ifndef BASIC_WIDGET_H
#define BASIC_WIDGET_H

#include <QWidget>

namespace Preview {
namespace Internal {

class BasicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasicWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

protected:
    void mousePressEvent(QMouseEvent *);

public:
    void setWidgetBackground(QColor color);

};

} // namespace Internal
} // namespace Preview


#endif // BASIC_WIDGET_H
