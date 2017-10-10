#ifndef PREVIEW_SLIDER_H
#define PREVIEW_SLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>
#include <QPainter>
#include <QStyleOptionSlider>

namespace Preview {
namespace Internal {

class Slider : public QSlider
{
    Q_OBJECT
public:
    Slider(QWidget *parent);
    ~Slider() {}

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void sliderClicked();

    // for slider paint timeInOut
public:
    void sliderRestoreDefault();
    void sliderSetTimeIn(quint16);
    void sliderSetTimeOut(quint16);

    void sliderSetTimeIn();
    void sliderSetTimeOut();

public:
    quint16 m_xPoint;
    bool m_isRestore;

private:
    bool m_timeInBtnClicked;
    bool m_timeOutBtnClicked;
    bool m_isClickTimeOutFirst;

    int m_timeInPointX;
    int m_timeOutPointX;
    int m_needPaintWidth;

};

} // namespace Internal
} // namespace Preview


#endif // VIDEOWIDGETSLIDER_H
