#ifndef VIDEO_CONTRL_WIDGET_H
#define VIDEO_CONTRL_WIDGET_H

#include <QWidget>
#include "videocontrlwidget_p.h"

namespace Monitor {
namespace Internal {

class VideoContrlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoContrlWidget(QWidget *parent = 0);
    ~VideoContrlWidget();

signals:

public slots:

private:
    void widgetInit();
    void setBackground(QColor color);

private:
    VideoContrlWidgetPrivate *d;

};


} // namespace Internal
} // namespace Monitor

#endif // VIDEO_CONTRL_WIDGET_H
