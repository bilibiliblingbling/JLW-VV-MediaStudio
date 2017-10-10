#include "sequencecanvas.h"

namespace TimeLine {
namespace Internal {

/*!
 * TODO: draw first/last frame of segments
 */

SequenceCanvas::SequenceCanvas(QwtPlot *plot)
    : QwtPlotCanvas(plot)
{
    setObjectName(QStringLiteral("TimeLine.SequenceCanvas"));

    setCursor(Qt::ArrowCursor);
}

} // Internal
} // TimeLine
