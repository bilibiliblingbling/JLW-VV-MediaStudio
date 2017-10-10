#include "controlwidget.h"
#include <QHBoxLayout>
#include <QDebug>

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
{
    resize(600,1000);
    tem = new tempWidget(this);
    tem->setGeometry(0,0,600,350);
    m_playBtn = new QPushButton(tr("play"),this);
    m_playBtn->setGeometry(0,360,50,30);
    m_stopBtn = new QPushButton (tr("stop"),this);
    m_stopBtn->setGeometry(60,360,50,30);
    m_pauseBtn = new QPushButton(tr("pause"),this);
    m_pauseBtn->setGeometry(120,360,50,30);
    m_skiptn = new QPushButton(tr("skip"),this);
    m_skiptn->setGeometry(180,360,50,30);
    m_skiptn->setCheckable(true);
    m_slider = new QSlider(Qt::Horizontal,this);
    m_slider->setGeometry(240,360,800,25);
    m_slider->setRange(0,1000);
    m_timer = new QTimer(this);

    connect(m_playBtn,SIGNAL(clicked(bool)),
            this,SLOT(onPlay()));
    connect(m_stopBtn,SIGNAL(clicked(bool)),
            this,SLOT(onStop()));
    connect(m_slider,SIGNAL(valueChanged(int)),
            this,SLOT(onValueChange(int)));
    connect(m_pauseBtn,SIGNAL(clicked(bool)),
            this,SLOT(onPause()));
    connect(m_skiptn,SIGNAL(clicked(bool)),
            this,SLOT(onSkip()));
    connect(tem->segPlay,SIGNAL(playFinished()),
            this,SLOT(onPlayEnd()));

    QMetaObject::Connection con;
    con = connect(tem->segPlay, SIGNAL(elapse(quint64)), this, SLOT(onUpdateTime(quint64)));
    Q_ASSERT(con);
    m_timer->setSingleShot(true);



}

void ControlWidget::onPlay()
{
    tem->segPlay->play();
    m_timer->start(1000);


}

void ControlWidget::onStop()
{
    qDebug()<<"clicked******clicked";
    tem->segPlay->stop();
}

void ControlWidget::onPause()
{
    tem->segPlay->pause();

}

void ControlWidget::onValueChange(int value)
{
    qulonglong range = tem->m_endTime - tem->m_bgnTime;
    qulonglong offset = (value*range)/1000;

    tem->segPlay->seek(offset);
}

void ControlWidget::onUpdateTime(quint64 t)
{
    qDebug()<<"t:"<<t<<endl;
    qDebug()<<"begin"<<tem->segPlay->beginTime();
    qDebug()<<"end"<<tem->segPlay->endTime();
    qDebug()<<"getCurrentPlayTime"<<tem->segPlay->currentTime();
    if (tem->segPlay->isPlaying()) {
        m_curTime = t;
        //qDebug()<<"m_curTime"<<m_curTime<<endl;
        //qDebug()<<"tem->m_bgnTime"<<tem->m_bgnTime<<endl;
        //int position = (m_curTime - tem->m_bgnTime)* 1000 / (tem->m_endTime - tem->m_bgnTime);
        //qDebug()<<"position:"<<position;
        //m_slider->setValue(position);
    }
}

void ControlWidget::onSkip()
{
    if (m_skiptn->isChecked()) {
        tem->segPlay->skip(DEFAULT_SKIP_PLAY_INTERVAL);
    } else {
        tem->segPlay->skip(0);
    }
}

void ControlWidget::onPlayEnd()
{
    qDebug()<<"play end"<<endl;
}


