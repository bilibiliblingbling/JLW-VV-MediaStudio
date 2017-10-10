#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QStackedWidget>
#include <QDateTime>
#include <QDebug>

#include "videoControlpanel.h"

namespace Preview {
namespace Internal {

#define TIME_RANGE (1000)

VideoControlPanel::VideoControlPanel(QWidget *parent)
    : BasicWidget(parent)
    , d(new VideoControlPanelPrivate())
    , m_currentPlayer(0)
    , m_playerData(NULL)
    , m_playBackData(NULL)
    , m_livePlayData(NULL)
    , m_isSeek(false)
{
    setMaximumHeight(28);
    panelInit();
}
void VideoControlPanel::panelInit()
{
    QFont ft;
    ft.setPixelSize(12);

    d->m_btnPlay  = new Utils::SkinButton(Resource::Play);
    d->m_btnPlay->setToolTip(tr("play"));
    connect(d->m_btnPlay, &Utils::SkinButton::clicked,
            this, &VideoControlPanel::playerPlay);
    d->m_btnPause = new Utils::SkinButton(Resource::Pause);
    d->m_btnPause->setToolTip(tr("pause"));
    connect(d->m_btnPause, &Utils::SkinButton::clicked,
            this, &VideoControlPanel::playerPause);

    d->m_btnStop  = new Utils::SkinButton(Resource::Stop);
    d->m_btnStop->setToolTip(tr("stop"));
    connect(d->m_btnStop, &Utils::SkinButton::clicked,
           [=](){playerStoped();});
    d->m_btnSetTimeIn = new Utils::SkinButton(Resource::TimeIn);
    d->m_btnSetTimeIn->setToolTip(tr("set TimeIn"));
    connect(d->m_btnSetTimeIn, SIGNAL(clicked(bool)),
            this, SLOT(on_setTimeInBtn_clicked()));
    d->m_btnSetTimeOut = new Utils::SkinButton(Resource::TimeOut);
    d->m_btnSetTimeOut->setToolTip(tr("set TimeOut"));
    connect(d->m_btnSetTimeOut, SIGNAL(clicked(bool)),
            this, SLOT(on_setTimeOutBtn_clicked()));
    d->m_btnClose  = new Utils::SkinButton(Resource::Close);
    d->m_btnClose->setToolTip(tr("close"));
    connect(d->m_btnClose, SIGNAL(clicked(bool)),
            this, SLOT(on_closeBtn_clicked()));
    //connect(d->m_btnClose, SIGNAL(clicked()), this, SIGNAL(CurrentPlayerClosed()));
    d->m_btnstatus = new Utils::SkinButton(Resource::LiveRpl);
#if 0
    d->m_timeLabel = new QLabel(tr("00:00:00"));
    d->m_timeLabel->setWindowFlags(Qt::FramelessWindowHint);
    d->m_timeLabel->setMargin(2);
    d->m_timeLabel->setFont(ft);
#endif

    d->m_timeEdit = new timeEdit;
    //connect(d->m_timeEdit, &timeEdit::editingFinished, this, &VideoControlPanel::timeEdited);

    d->m_slider = new Slider(this);
    d->m_slider->setRange(0, TIME_RANGE);
    connect(d->m_slider, SIGNAL(sliderClicked()),
            this, SIGNAL(currentOperatePanelClicked()));
    connect(d->m_slider, SIGNAL(sliderMoved(int)), this, SLOT(on_slider_moved(int)));
    connect(d->m_slider, &Slider::sliderPressed, [=](){m_isSeek = true;});
    // add a horizontalSpacer
    d->m_spacer = new QSpacerItem(1, 20, QSizePolicy::Expanding,
                                  QSizePolicy::Minimum);
    QHBoxLayout *hLayout = new QHBoxLayout;

    d->m_stackedWidget = new QStackedWidget;
    d->m_stackedWidget->setContentsMargins(0, 0, 0, 0);
    d->m_stackedWidget->setFixedSize(18, 15);
    d->m_stackedWidget->addWidget(d->m_btnPause);
    d->m_stackedWidget->addWidget(d->m_btnPlay);
    hLayout->addWidget(d->m_stackedWidget);
    hLayout->addWidget(d->m_btnStop);
    hLayout->addWidget(d->m_btnSetTimeIn);
    hLayout->addWidget(d->m_btnSetTimeOut);
    hLayout->addWidget(d->m_btnClose);
    hLayout->addWidget(d->m_btnstatus);
    hLayout->addItem(d->m_spacer);
    hLayout->addWidget(d->m_timeEdit);
    hLayout->setSpacing(1);
    hLayout->setMargin(0);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(d->m_slider);
    mainLayout->addLayout(hLayout);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
}

VideoControlPanel::~VideoControlPanel()
{
    delete d;
}

void VideoControlPanel::setPlayer(PlayerData *playerData)
{
    if (!playerData)
        return;

    if (m_currentPlayer)
        playerClosed();

    m_currentPlayer = playerData->getPlayer();
    m_playerData = playerData;

    m_playBackData = dynamic_cast<PlayBackData *>(playerData);
    if (!m_playBackData)
        m_livePlayData = dynamic_cast<LivePlayerData *>(playerData);

    d->m_timeEdit->setPlayerData(playerData);

    //just for playBack
    if (m_playBackData)
    {
        connect(m_currentPlayer.data(), &Player::IPlayer::elapse, this, &VideoControlPanel::onUpdateTime);
        connect(m_currentPlayer.data(), &Player::IPlayer::reachEnd, this, &VideoControlPanel::playerStoped);
    }
}

void VideoControlPanel::on_setTimeInBtn_clicked()
{
    if (!m_currentPlayer)
        return;

    //if (m_playerData->isPlayerInGroup)
    //if (m_playerData->playerInGroup())
    emit videoGroupSynOperate(TimeIn);

    //m_playerData->m_timeIn = m_currentPlayer->currentTime();
    m_playerData->setTimeIn(m_currentPlayer->currentTime());

    qDebug() << "SetTimeIn: " << m_playerData->getTimeIn() << ": " << m_playBackData->getTimeIn();

    d->m_slider->sliderSetTimeIn();
}

void VideoControlPanel::on_setTimeOutBtn_clicked()
{
    if (!m_currentPlayer)
        return;

    //if (m_playerData->isPlayerInGroup)
    //if (m_playerData->playerInGroup())
    emit videoGroupSynOperate(TimeOut);

    //m_playerData->m_timeOut = m_currentPlayer->currentTime();
    m_playerData->setTimeOut(m_currentPlayer->currentTime());

    d->m_slider->sliderSetTimeOut();
}

void VideoControlPanel::on_slider_moved(int timePosition)
{
    if (!m_currentPlayer)
        return;

    if (!m_playBackData)
        return;

    qulonglong offset = (timePosition * m_playBackData->getTimeRange()) / TIME_RANGE;
#if 0
    QString cur = QDateTime::fromTime_t(m_playerData->m_bgnTime + offset).toString("hh:mm:ss");
    d->m_timeLabel->setText(cur);
#endif
    d->m_timeEdit->setEditTime(m_playBackData->getBgnTime() + offset);
    m_currentPlayer->seek(offset);
}

void VideoControlPanel::on_closeBtn_clicked()
{
    playerClosed();

    if (m_playBackData)
    {
        disconnect(m_currentPlayer.data(), &Player::IPlayer::elapse, this, &VideoControlPanel::onUpdateTime);
        disconnect(m_currentPlayer.data(), &Player::IPlayer::reachEnd, this, &VideoControlPanel::playerStoped);
    }

    emit currentPlayerClosed();

    m_currentPlayer.reset();
}

void VideoControlPanel::onUpdateTime(quint64 timeRange)
{
    if (!m_playBackData)
        return;

    if (m_isSeek)
    {
        m_isSeek = false;
        return;
    }

    quint64 currentTime;
    currentTime = m_playBackData->getBgnTime() + timeRange;
    double position = timeRange * TIME_RANGE / m_playBackData->getTimeRange();
    d->m_slider->setValue(position);

    quint16 pos;
    pos = (position * d->m_slider->width()) / TIME_RANGE;
    d->m_slider->m_xPoint = pos;

#if 0
    if (currentTime)
        d->m_timeLabel->setText(QDateTime::fromTime_t(currentTime).toString("hh:mm:ss"));
    else
        d->m_timeLabel->setText("00:00:00");
#endif

    d->m_timeEdit->setEditTime(currentTime);
}

void VideoControlPanel::videoGroupSynCtrl(GroupOperateType t)
{
    if (!m_currentPlayer)
        return;

    if (t == TimeIn)
    {
        m_playerData->setTimeIn(m_currentPlayer->currentTime()); // save data
        d->m_slider->sliderSetTimeIn(); // paint slider
    }
    else if (t == TimeOut)
    {
        m_playerData->setTimeOut(m_currentPlayer->currentTime());
        d->m_slider->sliderSetTimeOut();
    }
    qDebug() << "VideoOperatePanel::videoGroupSynCtrl() entered!";
}

void VideoControlPanel::playerPlay(bool isPlay)
{
    qDebug() << "playerPlay:" << isPlay;
    if (!isPlay)
    {
        d->m_stackedWidget->setCurrentIndex(0);
        m_currentPlayer->play();
    }
}

void VideoControlPanel::playerPause(bool isPause)
{
     qDebug() << "playerPause:" << isPause;
    if (!isPause)
    {
        d->m_stackedWidget->setCurrentIndex(1);
        m_currentPlayer->pause();
    }
}

void VideoControlPanel::playerStoped()
{
    m_currentPlayer->stop();
    d->m_slider->setValue(0);
    d->m_stackedWidget->setCurrentIndex(1);
    //d->m_timeLabel->setText("00:00:00");
    d->m_timeEdit->setEditTime(0);
}

void VideoControlPanel::playerClosed()
{
    playerStoped();
    d->m_stackedWidget->setCurrentIndex(0);
    // clear timeIn and timeOut and resetPlayer
    m_playerData->clearTimeInOut();
    m_playerData->resetPlayer();

    if (d->m_slider->m_isRestore)
        return;

    //restore slider it's default styleSheet
    d->m_slider->sliderRestoreDefault();
}

#if 0
void VideoControlPanel::timeEdited()
{
    QString editTime = d->m_timeEdit->text();

}
#endif

/*
 * quint16 pos;
    pos = (position * d->m_slider->width()) / TIME_RANGE;
    d->m_slider->m_xPoint = pos;
 * */

void VideoControlPanel::setTimeIn()
{
    quint16 pos = 0;
    if (!m_playBackData)
        return; //TODO for LiveMode
    // just for PlayBack
    if (m_playerData->getTimeIn())
        pos = (m_playerData->getTimeIn() - m_playerData->getBgnTime()) * d->m_slider->width() / m_playBackData->getTimeRange();

    qDebug() << "pos : " << pos << "TimeIn: " << m_playerData->getTimeIn() << m_playBackData->getTimeRange();
    d->m_slider->sliderSetTimeIn(pos);
}

void VideoControlPanel::setTimeOut()
{
    quint16 pos = 0;

    if (!m_playBackData)
        return; //TODO for LiveMode
    // just for PlayBack
    if (m_playerData->getTimeOut())
        pos = (m_playerData->getTimeOut() - m_playerData->getBgnTime()) * d->m_slider->width() / m_playBackData->getTimeRange();

    d->m_slider->sliderSetTimeOut(pos);
}
#if 0

void VideoControlPanel::clearTimeIn()
{
    d->m_slider->m_xPoint = 0;
    d->m_slider->m_timeInBtnClicked = true;
    d->m_slider->update();
}

void VideoControlPanel::clearTimeOut()
{
    d->m_slider->m_xPoint = 0;
    d->m_slider->m_timeOutBtnClicked = true;
    d->m_slider->update();
}
#endif

void VideoControlPanel::clearTimeInOut()
{
    // clear timeIn and timeOut
    m_playerData->clearTimeInOut();

    //restore slider it's default styleSheet
    d->m_slider->sliderRestoreDefault();
}

} // namespace Internal
} // namespace Preview
