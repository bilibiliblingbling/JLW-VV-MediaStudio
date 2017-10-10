#include "constants.h"
#include "sequencecanvas.h"
#include "sequencetimepicker.h"
#include "sequencewidget.h"

#include <QBoxLayout>
#include <QLCDNumber>
#include <QScrollBar>

#include <qwt/qwt_date_scale_draw.h>
#include <qwt/qwt_date_scale_engine.h>
#include <qwt/qwt_slider.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_wheel.h>

#include <utils/tmplog.h>

namespace TimeLine {
namespace Internal {

class SequenceWidgetPrivate
{
public:
#if 0
    QwtSlider *m_slider;
#else
    QwtWheel *m_wheel;
    QLCDNumber *m_lcd;
#endif

    QwtPlot *m_plot;
    QScrollBar *m_scrollBar;
};

static SequenceWidget *s_sequencewidget = 0;

const double MIN_TIME                       =       0.0;
const double MAX_TIME                       = 3600000.0;

const double MAJOR_TICK_LENGTH              =      17.0;
const double MINOR_TICK_LENGTH              =      11.0;

const double MIN_ZOOM_FACTOR                =       1.0;
const double MAX_ZOOM_FACTOR                =     225.0;
const double SINGLE_ZOOM_STEP               =       0.1;

SequenceWidget::SequenceWidget(QWidget *parent)
    : QWidget(parent)
    , d(new SequenceWidgetPrivate())
{
    setObjectName(QStringLiteral("TimeLine.SequenceWidget"));
    setMinimumHeight(200);

    // slider or wheel
#if 0
    d->m_slider = new QwtSlider();
    d->m_slider->setOrientation(Qt::Horizontal);
    d->m_slider->setLowerBound(1.0);
    d->m_slider->setLowerBound(255.0);
    d->m_slider->setTracking(true);
    d->m_slider->setTotalSteps(2540);
#else
    d->m_wheel = new QwtWheel();
    d->m_wheel->setOrientation(Qt::Horizontal);
    d->m_wheel->setRange(MIN_ZOOM_FACTOR, MAX_ZOOM_FACTOR);
    d->m_wheel->setSingleStep(SINGLE_ZOOM_STEP);
    d->m_wheel->setMass(SINGLE_ZOOM_STEP);
    d->m_wheel->setValue(MIN_ZOOM_FACTOR);

    d->m_lcd = new QLCDNumber();
    d->m_lcd->display(d->m_wheel->value());

    connect(d->m_wheel, &QwtWheel::valueChanged,
            this, [this](double value) { d->m_lcd->display(value); zoomPlot(value); });
#endif

    initPlot();
    initScrollBar();

    QVBoxLayout *layoutLeft = new QVBoxLayout();
    layoutLeft->addWidget(new QWidget(), 1);
#if 0
    layoutLeft->addWidget(d->m_slider);
#else
    layoutLeft->addWidget(d->m_wheel);
    layoutLeft->addWidget(d->m_lcd);
#endif

    QVBoxLayout *layoutRight = new QVBoxLayout();
    layoutRight->addWidget(d->m_plot);
    layoutRight->addWidget(d->m_scrollBar);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 8, 0, 0);
    mainLayout->addLayout(layoutLeft);
    mainLayout->addLayout(layoutRight);
    setLayout(mainLayout);

    s_sequencewidget = this;
}

SequenceWidget::~SequenceWidget()
{
    s_sequencewidget = 0;

    delete d;
}

SequenceWidget *SequenceWidget::instance()
{
    return s_sequencewidget;
}

void SequenceWidget::initPlot()
{
    // plot
    d->m_plot = new QwtPlot();

    SequenceCanvas *canvas = new SequenceCanvas();
    d->m_plot->setCanvas(canvas);
#if 0
    d->m_plot->plotLayout()->setAlignCanvasToScale(QwtPlot::yLeft, true);
#endif

    for (int i = 0; i < QwtPlot::axisCnt; ++i)
        d->m_plot->enableAxis(i, (i == QwtPlot::xTop));

    QwtDateScaleDraw *scaleDraw = new QwtDateScaleDraw(Qt::UTC);
    scaleDraw->setTickLength(QwtScaleDiv::MajorTick, MAJOR_TICK_LENGTH);
    scaleDraw->setTickLength(QwtScaleDiv::MinorTick, MINOR_TICK_LENGTH);
    scaleDraw->setDateFormat(QwtDate::Millisecond, QStringLiteral("hh:mm:ss.zzz"));
    scaleDraw->setDateFormat(QwtDate::Second, QStringLiteral("hh:mm:ss"));
    scaleDraw->setDateFormat(QwtDate::Minute, QStringLiteral("hh:mm:ss"));
    scaleDraw->setDateFormat(QwtDate::Hour, QStringLiteral("hh:mm:ss"));
    d->m_plot->setAxisScaleDraw(QwtPlot::xTop, scaleDraw);


    QwtDateScaleEngine *scaleEngine = new QwtDateScaleEngine(Qt::UTC);
    d->m_plot->setAxisScaleEngine(QwtPlot::xTop, scaleEngine);

    d->m_plot->setAxisScale(QwtPlot::xTop, MIN_TIME, MAX_TIME);

    SequenceTimePicker *timePicker = new SequenceTimePicker(canvas);
    Q_UNUSED(timePicker);
}

void SequenceWidget::initScrollBar()
{
    // scroll bar
    d->m_scrollBar = new QScrollBar(Qt::Horizontal);
    updateScrollBar();
    connect(d->m_scrollBar, &QScrollBar::valueChanged,
            this, [this](int value) { panPlot(value*1000.); });
}

void SequenceWidget::zoomPlot(double value)
{
    d->m_plot->updateAxes();
    QwtScaleDiv scaleDiv = d->m_plot->axisScaleDiv(QwtPlot::xTop);
    double range = (MAX_TIME-MIN_TIME) / value;
    if (scaleDiv.lowerBound()+range > MAX_TIME) {
        scaleDiv.setUpperBound(MAX_TIME);
        scaleDiv.setLowerBound(MAX_TIME-range);
    } else {
        scaleDiv.setUpperBound(scaleDiv.lowerBound()+range);
    }
    updateAxisScale(scaleDiv);

    updateScrollBar();
}

void SequenceWidget::panPlot(double value)
{
    d->m_plot->updateAxes();
    QwtScaleDiv scaleDiv = d->m_plot->axisScaleDiv(QwtPlot::xTop);
    scaleDiv.setInterval(scaleDiv.lowerBound()+value, scaleDiv.upperBound()+value);
    updateAxisScale(scaleDiv);

    updateScrollBar();
}

void SequenceWidget::updateAxisScale(const QwtScaleDiv &scaleDiv)
{
    const bool autoReplot = d->m_plot->autoReplot();
    d->m_plot->setAutoReplot(false);
    d->m_plot->setAxisScale(QwtPlot::xTop, scaleDiv.lowerBound(), scaleDiv.upperBound());
    d->m_plot->setAutoReplot(autoReplot);
    d->m_plot->replot();
}

void SequenceWidget::updateScrollBar() const
{
    d->m_scrollBar->blockSignals(true);
    d->m_plot->updateAxes();
    const QwtScaleDiv &scaleDiv = d->m_plot->axisScaleDiv(QwtPlot::xTop);
    double left = MIN_TIME - scaleDiv.lowerBound();
    double right = MAX_TIME - scaleDiv.upperBound();
    d->m_scrollBar->setRange(left/1000, right/1000);
    d->m_scrollBar->setValue(0);
    d->m_scrollBar->blockSignals(false);
}

} // Internal
} // TimeLine
