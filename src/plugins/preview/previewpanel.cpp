#include <QFont>
#include <QAction>
#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>
#include <core/icore.h>
#include <core/pane.h>
#include <extensionsystem/pluginmanager.h>
#include <player/iplayer.h>
#include <player/iplayerfactory.h>

#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGridLayout>
#include <projectexplorer/metamimedata.h>
#include <projectexplorer/metadata.h>
#include <resource/resource.h>

#include "constants.h"
#include "ipreviewlayoutfactory.h"
#include "inouteditdialog.h"
#include "previewpanel.h"
#include "videogroupmanagerdlg.h"

namespace Preview {
namespace Internal {

static PreviewPanel *s_previewPanel = 0;

PreviewPanel::PreviewPanel(QWidget *parent)
    : BasicWidget(parent)
    , m_currentWidget(0)
    , m_layoutMenu(new QMenu(tr("Preview Layout")))
    , m_playerData(NULL)
{
    setObjectName(QLatin1String("Preview.PreviewPanel"));

    setMinimumHeight(240);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(contextMenu(const QPoint &)));

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(1);
    setLayout(mainLayout);

    setAcceptDrops(true);
    setMouseTracking(true);
    setWidgetBackground(Qt::black);

    Core::ICore::previewPane()->setWidget(this);
    s_previewPanel = this;
}

PreviewPanel::~PreviewPanel()
{
    s_previewPanel = 0;
    m_currentWidget = 0;

    qDeleteAll(m_videoWidgets);
}

PreviewPanel *PreviewPanel::instance()
{
    return s_previewPanel;
}

Core::Id PreviewPanel::activeLayout()
{
    return s_previewPanel->m_activeLayout;
}

void PreviewPanel::loadLayout(Core::Id id)
{
    if(s_previewPanel->m_activeLayout == id)
        return;

    qint32 oldCount = 0;
    qint32 newCount = 0;
    IPreviewLayoutFactory *oldFactory = PreviewLayoutFactoryManager::
            factory(s_previewPanel->m_activeLayout);
    if (oldFactory && oldFactory->isValid())
    {
        oldCount = oldFactory->rows() * oldFactory->columns();
        for(int i = 0; i < oldCount; ++i)
        {
            PreviewVideoWidget *w = s_previewPanel->m_videoWidgets.at(i);
            w->setVisible(false);
            s_previewPanel->layout()->removeWidget(w);
        }
    }

    s_previewPanel->m_activeLayout = id;

    IPreviewLayoutFactory *newFactory = PreviewLayoutFactoryManager::factory(id);
    if (newFactory && newFactory->isValid())
    {
        newCount = newFactory->rows() * newFactory->columns();
        while (s_previewPanel->m_videoWidgets.count() < newCount)
        {
            s_previewPanel->m_videoWidgets.append(new PreviewVideoWidget(s_previewPanel));
        }

        QGridLayout *gridlayout = qobject_cast<QGridLayout*>(s_previewPanel->layout());
        for (int r = 0; r < newFactory->rows(); ++r)
        {
            for (int c = 0; c < newFactory->columns(); ++c)
            {
                int idx = r * newFactory->columns() + c;
                PreviewVideoWidget *w = s_previewPanel->m_videoWidgets.at(idx);
                connect(w, &PreviewVideoWidget::widgetClicked,
                        s_previewPanel, &PreviewPanel::setWidgetFocus);
                connect(w, &PreviewVideoWidget::videoGroupSynOperateSignal,
                        s_previewPanel, &PreviewPanel::videoGroupOperate);
                w->setVisible(true);
                gridlayout->addWidget(w, r, c);
            }
        }
    }
}

void PreviewPanel::updateLayoutMenu(QMenu *menu)
{
    QActionGroup *ag = new QActionGroup(menu);
    connect(ag, &QActionGroup::triggered, &PreviewPanel::setPreviewLayout);

    foreach (auto factory, PreviewLayoutFactoryManager::factories())
    {
        QAction *act = ag->addAction(factory->name());
        act->setData(factory->id().toSetting());
        act->setCheckable(true);

        // set current layout checked
        if(factory->id() == PreviewPanel::activeLayout())
            act->setChecked(true);
    }
    menu->addActions(ag->actions());
    menu->setEnabled(true);
}

void PreviewPanel::setPreviewLayout(QAction *action)
{
    Core::Id id = Core::Id::fromSetting(action->data());

    // check active layout
    if (id != PreviewPanel::activeLayout())
    {
        PreviewPanel::loadLayout(id);
    }
}

void PreviewPanel::videoGroupContextMenu(QMenu *menu)
{
    QMenu * groupMenu = new QMenu(tr("Video Group"));
    quint8 group = m_currentWidget->m_groupIndex;

    QActionGroup *ag = new QActionGroup(groupMenu);
    connect(ag, &QActionGroup::triggered, this, &PreviewPanel::setCurrentWidgetIngroup);
    for (int i = 0; i < 9; ++i)
    {
        QIcon icon = Resource::PixmapManager
                ::inst().getPixmap(Resource::GroupIcons, i);
        QAction *act = ag->addAction(icon, tr("group%1").arg(i));
        act->setData(i);
        act->setCheckable(true);

        // set currentWidget in which group
        if (i == group)
            act->setChecked(true);
    }

    groupMenu->addActions(ag->actions());
    groupMenu->setEnabled(true);

    menu->addMenu(groupMenu);
}

void PreviewPanel::setCurrentWidgetIngroup(QAction *action)
{
    QList<QColor> colorList;
    colorList  << Qt::lightGray << Qt::blue << Qt::red
               << Qt::green << Qt::yellow << Qt::magenta
               << Qt::cyan << "#A0522D" << Qt::darkGreen;

    qint8 actIndex = (action->data()).toInt();

    m_currentWidget->setVideoInGroup(colorList[actIndex]);
    m_currentWidget->m_groupIndex = actIndex;
}


void PreviewPanel::contextMenu(const QPoint & pos)
{
    m_layoutMenu->clear();
    m_playerData = m_currentWidget->getCurrentPlayerData();

    QMenu *contextmenu = new QMenu(this);
    QAction *inOutEidtAct = new QAction(tr("Time-inOut Eidt"), contextmenu);
    inOutEidtAct->setEnabled(false);
    contextmenu->addAction(inOutEidtAct);

    connect(inOutEidtAct, SIGNAL(triggered(bool)), this, SLOT(inOutEditDialog()));
    contextmenu->addSeparator();

    QAction *clearTimeIn = new QAction(tr("Clear TimeIn"), contextmenu);
    contextmenu->addAction(clearTimeIn);
    clearTimeIn->setEnabled(false);
    connect(clearTimeIn, SIGNAL(triggered(bool)),
            this, SLOT(clearCurrentWidgetTimeIn()));
    QAction *clearTimeOut = new QAction(tr("Clear TimeOut"), contextmenu);
    contextmenu->addAction(clearTimeOut);
    clearTimeOut->setEnabled(false);
    connect(clearTimeOut, SIGNAL(triggered(bool)),
            this, SLOT(clearCurrentWidgetTimeOut()));
    QAction *clearAll = new QAction (tr("Clear TimeIn-Out"), contextmenu);
    contextmenu->addAction(clearAll);
    clearAll->setEnabled(false);
    connect(clearAll, SIGNAL(triggered(bool)), this, SLOT(clearTimeInOut()));
    contextmenu->addSeparator();

    QAction *locateTimeIn = new QAction(tr("Locate TimeIn"), contextmenu);
    contextmenu->addAction(locateTimeIn);
    locateTimeIn->setEnabled(false);
    connect(locateTimeIn, SIGNAL(triggered(bool)),this, SLOT(playerLocateTimeIn()));
    QAction *locateTimeOut = new QAction(tr("Locate TimeOut"), contextmenu);
    contextmenu->addAction(locateTimeOut);
    locateTimeOut->setEnabled(false);
    connect(locateTimeOut, SIGNAL(triggered(bool)), this, SLOT(playerLocateTimeOut()));

    contextmenu->addSeparator();
    //同步打点分组...
    contextmenu->addAction(tr("Group Operate"),
                           this, SLOT(videoGroupManagerDialog()));
    contextmenu->addSeparator();
    contextmenu->addMenu(m_layoutMenu);

    updateLayoutMenu(m_layoutMenu);

    contextmenu->addSeparator();
    videoGroupContextMenu(contextmenu);

    if (m_playerData)
    {
        if (m_playerData->getPlayer())
            inOutEidtAct->setEnabled(true);

        if (m_playerData->getTimeIn() || m_playerData->getTimeOut())
        {
            if (m_playerData->getTimeIn())
                clearTimeIn->setEnabled(true);
            if (m_playerData->getTimeOut())
                clearTimeOut->setEnabled(true);

            locateTimeIn->setEnabled(true);
            locateTimeOut->setEnabled(true);
            clearAll->setEnabled(true);
        }
    }
    contextmenu->exec(mapToGlobal(pos));
}

void PreviewPanel::inOutEditDialog()
{
    InOutEditDialog dialog(this, m_playerData, m_currentWidget);
    dialog.exec();
}

void PreviewPanel::videoGroupManagerDialog()
{
    VideoGroupManagerDlg dialog(this);
    dialog.exec();

    if (!m_playerData)
        return;

    if (m_playerData->getPlayer() && m_currentWidget->m_groupIndex)
        m_playerData->setPlayerInGroup(true);
}

QList<PreviewVideoWidget *> PreviewPanel::getVideoWidgets()
{
    return m_videoWidgets;
}

qint8 PreviewPanel::getWidgetsCount() const
{
    return layout()->count();
}

void PreviewPanel::setWidgetFocus(PreviewVideoWidget * child)
{
    if (!child)
        return ;

    if (m_currentWidget)
    {
        if (m_currentWidget == child)
            return;
        else
            setCurrentWidgetFocus(false);
    }

    m_currentWidget = child;
    //m_playerData = m_currentWidget->getCurrentPlayerData();
    setCurrentWidgetFocus(true);
}

void PreviewPanel::setCurrentWidgetFocus(bool iscurrentWidget)
{
    if(iscurrentWidget)
        m_currentWidget->setVideoFocus(true);
    else
        m_currentWidget->setVideoFocus(false);
}

void PreviewPanel::videoGroupOperate(PreviewVideoWidget *child, GroupOperateType t)
{
    if (!child)
        return ;

    quint8 groupIndex = child->m_groupIndex;

    for (int i = 0; i < layout()->count(); ++i)
    {
        PreviewVideoWidget *w = m_videoWidgets.at(i);
        if (w == child) //itself
            continue;

        if (w->m_groupIndex == groupIndex)
        {
            // currentWidget in group first, and then have a player
            if (w->getCurrentPlayerData())
                w->videoGroupSynCtrl(t);
        }
    }
}

void PreviewPanel::clearCurrentWidgetTimeIn()
{
    m_playerData->clearTimeIn(); // clear data
    m_currentWidget->setTimeIn(); // repaint slider
}

void PreviewPanel::clearCurrentWidgetTimeOut()
{
    m_playerData->clearTimeOut();
    m_currentWidget->setTimeOut();
}

void PreviewPanel::clearTimeInOut()
{
    m_playerData->clearTimeInOut();
    m_currentWidget->clearTimeInOut();
}

void PreviewPanel::playerLocateTimeIn()//has a bug
{
    /*// play playBack
     *
     * when player play reach the end, and this player already has a timeIn,
     * then I want to locate to the TimeIn time, in this situation, there has a bug, player
     * only exec play(), not exec seek() to Timein;
     * */

    quint64 offset = 0;
    bool isPlayerPlaying = m_playerData->getPlayer()->isPlaying();

    qDebug() << "isPlayerPlaying : " << isPlayerPlaying;
    if (!isPlayerPlaying) // when player play reach end
        m_playerData->getPlayer()->play();

    if (m_playerData->getTimeIn())
        offset = m_playerData->getTimeIn() - m_playerData->getBgnTime();

    qDebug() << "playerLoacateTimeIn: offset " << offset;

    m_playerData->getPlayer()->seek(offset);
}

void PreviewPanel::playerLocateTimeOut()
{
    auto playBack = dynamic_cast<PlayBackData *>(m_playerData);

    if (!playBack)
        return; // TODO ,just for playBack

    quint64 offset = playBack->getTimeRange();
    if (m_playerData->getTimeOut())
        offset = m_playerData->getTimeOut() - m_playerData->getBgnTime();

    m_playerData->getPlayer()->seek(offset);
}

} // namespace Internal
} // namespace Preview
