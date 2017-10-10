#include "sequencetimepicker.h"

#include <qwt/qwt_date.h>
#include <qwt/qwt_picker_machine.h>

namespace TimeLine {
namespace Internal {

SequenceTimePicker::SequenceTimePicker(QWidget *canvas)
    : QwtPlotPicker(canvas)
{
    setObjectName(QStringLiteral("TimeLine.SequenceTimePicker"));

    setTrackerMode(QwtPlotPicker::ActiveOnly);
    setRubberBand(VLineRubberBand);
    setRubberBandPen(QPen(Qt::red));

    setStateMachine(new QwtPickerTrackerMachine());
}

QwtText SequenceTimePicker::trackerTextF(const QPointF &pos) const
{
    return QwtText(QwtDate::toDateTime(pos.x()).toString(QStringLiteral("hh:mm:ss")));
}

} // Internal
} // TimeLine
