#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include "tempwidget.h"
#include <QTimer>


#define DEFAULT_SKIP_PLAY_INTERVAL  (5)


class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);

signals:

protected slots:
    void onStop();
    void onPlay();
    void onPause();
    void onValueChange(int);
    void onUpdateTime(quint64 t);
    void onSkip();
    void onPlayEnd();

private:
    QPushButton *m_playBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_pauseBtn;
    QPushButton *m_skiptn;
    tempWidget *tem;
    QSlider *m_slider;
    QTimer *m_timer;
    quint64 m_curTime;
};

#endif // CONTROLWIDGET_H
