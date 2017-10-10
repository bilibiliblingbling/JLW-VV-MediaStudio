#include <QDebug>
#include <QStyleOptionFocusRect>
#include <QPainter>
#include <QPalette>
#include <QVBoxLayout>
#include "constants.h"
#include "previewvideowidget.h"

#include "playerwidget.h"

namespace Preview {
namespace Internal {

/*! previewVIdeoWidget
 *  +----------------------+
 *  |                      |
 *  |    playerWidget      |
 *  |                      |
 *  |----------------------|
 *  |   VideoControlPanel  |
 *  |----------------------|
 *  |  videoInWhichGroup   |
 *  +----------------------+
 *  splitter
 */


namespace PI = Player;//import Player::IPlayer::PlayMode

PreviewVideoWidget::PreviewVideoWidget(QWidget *parent)
    : QSplitter(parent)
    , m_groupIndex(0)
    , m_videoControlPanel(0)
    , m_timer(0)
    , m_videoWidget(0)
    , m_videoInWhichGroup(0)
    , m_isPlayerCreated(false)
    , m_playerData(NULL)
{
    setObjectName(QStringLiteral("PreviewVideoWidget"));
    setChildrenCollapsible(false);
    setOrientation(Qt::Vertical);
    setHandleWidth(0);

    m_videoWidget = new PlayerWidget(this);
    connect(m_videoWidget, &PlayerWidget::currentWidgetAcceptDropEvent,[=](){
        emit widgetClicked(this);
    });
    connect(m_videoWidget, &PlayerWidget::createPlayerSuccessed,
            this, &PreviewVideoWidget::loadVideoControlPanel);

    m_videoInWhichGroup = new BasicWidget(this);
    m_videoInWhichGroup->setFixedHeight(3);
    m_videoInWhichGroup->setWidgetBackground(Qt::lightGray);

    addWidget(m_videoWidget);
    addWidget(m_videoInWhichGroup);

#if 1
    m_videoControlPanel = new VideoControlPanel(this);
    connect(m_videoControlPanel, &VideoControlPanel::currentOperatePanelClicked,
            [=]() {emit widgetClicked(this); });
    connect(m_videoControlPanel, &VideoControlPanel::videoGroupSynOperate,
            [=](GroupOperateType t) {
        if(m_groupIndex)
            emit videoGroupSynOperateSignal(this, t);
    });
    connect(m_videoControlPanel, &VideoControlPanel::currentPlayerClosed, this, &PreviewVideoWidget::playerClosed);
    m_videoControlPanel->setWidgetBackground(Qt::lightGray);
    m_videoControlPanel->setVisible(false);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout,
            [=]() { m_videoControlPanel->setVisible(false);});
#endif
    setContentsMargins(1,1,1,0);
    setBackground(Qt::lightGray);

    setAcceptDrops(true);
    //setMouseTracking(true);
}

PreviewVideoWidget::~PreviewVideoWidget()
{
    delete m_playerData;
    m_playerData = NULL;
}

void PreviewVideoWidget::setVideoFocus(bool bFocus)
{
    if (bFocus)
        setBackground(Qt::yellow);
    else
        setBackground(Qt::lightGray);
}

void PreviewVideoWidget::setVideoInGroup(QColor color)
{
    m_videoInWhichGroup->setWidgetBackground(color);
}

void PreviewVideoWidget::loadVideoControlPanel(const Player::IPlayerPtr &player)
{
    insertWidget(1, m_videoControlPanel);
    m_videoControlPanel->setVisible(true);

    if (m_playerData)
    {
        delete m_playerData;
        m_playerData = NULL;
    }

    PI::IPlayer::PlayMode mode = player->playMode();

    if (PI::IPlayer::PlaybackMode == mode)
        m_playerData = new PlayBackData();
    else if (PI::IPlayer::LiveMode == mode)
        m_playerData = new LivePlayerData();

    m_playerData->setPlayer(player);
    m_playerData->setData();

#if 0
    m_playerData->m_player = player;
    m_playerData->m_bgnTime = player->beginTime();
    m_playerData->m_endTime = player->endTime();
    m_playerData->m_timeRange = m_playerData->m_endTime - m_playerData->m_bgnTime;
    m_playerData->m_timeIn = 0;
    m_playerData->m_timeOut = 0;
    m_playerData->isPlayerInGroup = false;
#endif

    m_videoControlPanel->setPlayer(m_playerData);
    m_currentPlayer = player;
    m_isPlayerCreated = true;
}

void PreviewVideoWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    if (!m_isPlayerCreated)
        return;

    m_timer->stop();
    m_videoControlPanel->setVisible(true);
}

void PreviewVideoWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    if (!m_isPlayerCreated)
        return;

    m_timer->start(1000);
}

void PreviewVideoWidget::mousePressEvent(QMouseEvent *ev)
{
    emit widgetClicked(this);
    QSplitter::mousePressEvent(ev);
}

void PreviewVideoWidget::videoGroupSynCtrl(GroupOperateType t)
{
    if (!m_currentPlayer)
        return;

    m_videoControlPanel->videoGroupSynCtrl(t);
}

PlayerData *PreviewVideoWidget::getCurrentPlayerData() const
{
    return m_playerData;
}

void PreviewVideoWidget::playerClosed()
{
    m_videoWidget->setAttribute(Qt::WA_PaintOnScreen, false);

    m_videoControlPanel->setVisible(false);
    this->setBackground(Qt::lightGray);
    m_isPlayerCreated = false;
}

void PreviewVideoWidget::setBackground(QColor color)
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void PreviewVideoWidget::setTimeIn()
{
    m_videoControlPanel->setTimeIn();
}

void PreviewVideoWidget::setTimeOut()
{
    m_videoControlPanel->setTimeOut();
}

void PreviewVideoWidget::clearTimeInOut()
{
    m_videoControlPanel->clearTimeInOut();
}

} // namespace Internal
} // namespace Preview
