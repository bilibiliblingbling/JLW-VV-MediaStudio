#ifndef SEQUENCE_CANVAS_H
#define SEQUENCE_CANVAS_H

#include <qwt/qwt_plot_canvas.h>

namespace TimeLine {
namespace Internal {

class SequenceCanvas : public QwtPlotCanvas
{
public:
    explicit SequenceCanvas(QwtPlot * = NULL);
};

} // Internal
} // TimeLine

#endif // SEQUENCE_CANVAS_H
