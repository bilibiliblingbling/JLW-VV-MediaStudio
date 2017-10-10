#ifndef SEQUENCE_TIME_PICKER_H
#define SEQUENCE_TIME_PICKER_H

#include <qwt/qwt_plot_picker.h>

namespace TimeLine {
namespace Internal {

class SequenceTimePicker : public QwtPlotPicker
{
    Q_OBJECT

public:
    explicit SequenceTimePicker(QWidget *);

protected:
    virtual QwtText trackerTextF(const QPointF &) const;
};

} // Internal
} // TimeLine

#endif // SEQUENCE_TIME_PICKER_H
