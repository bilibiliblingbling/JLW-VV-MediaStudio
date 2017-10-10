#ifndef SEQUENCE_WIDGET_H
#define SEQUENCE_WIDGET_H

#include "timeline_global.h"

#include <qwidget.h>

class QwtScaleDiv;
namespace TimeLine {
namespace Internal {

class SequenceWidgetPrivate;
class SequenceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SequenceWidget(QWidget *parent = 0);
    ~SequenceWidget();

    static SequenceWidget *instance();

private:
    void initPlot();
    void initScrollBar();

    void zoomPlot(double value);
    void panPlot(double value);

    void updateAxisScale(const QwtScaleDiv &scaleDiv);
    void updateScrollBar() const;

private:
    SequenceWidgetPrivate *d;
};

} // Internal
} // TimeLine

#endif // SEQUENCE_WIDGET_H
