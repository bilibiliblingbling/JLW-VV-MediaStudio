#include <QAction>
#include <QBoxLayout>
#include <QComboBox>
#include <QCoreApplication>
#include <QLabel>
#include <QStackedWidget>
#include <QToolButton>

#include <core/constants.h>
#include <core/action/actionmanager.h>
#include <core/commandcombobox.h>
#include <core/icontext.h>
#include <core/icore.h>
#include <core/minisplitter.h>
#include <core/pane.h>
#include <projectexplorer/mediasource.h>
#include <utils/styledbar.h>

#include "browsepanewidget.h"
#include "constants.h"
#include "imediasourcefactory.h"
#include "querywidget.h"
#include "treewidget.h"

namespace MediaSource {
namespace Internal {

class NavComboBox : public Core::CommandComboBox
{
public:
    explicit NavComboBox(BrowsePaneWidget *browsepane)
        : m_browsePaneWidget(browsepane)
    {}

private:
    Core::Command *command(const QString &text) const
    {
        return m_browsePaneWidget->command(text);
    }

private:
    BrowsePaneWidget *m_browsePaneWidget;
};


const int IDX_DEVICE_WIDGET                 = 0;
const int IDX_QUERY_WIDGET                  = 1;

static BrowsePaneWidget *s_browsepanewidget = 0;
BrowsePaneWidget::BrowsePaneWidget()
    : m_context(new Core::IContext(this))
    , m_toolbar(new Utils::StyledBar(this))
    , m_comboBox(new NavComboBox(this))
    , m_closeButton(new QToolButton(this))
    , m_stackedWidget(new QStackedWidget(this))
    , m_deviceWidget(new TreeWidget(this))
    , m_queryWidget(new QueryWidget(this))
{
    setObjectName(QLatin1String("MediaSource.BrowsePaneWidget"));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Core::Context context(Constants::C_MEDIASOURCE_BROWSEPANEWIDGET);

    m_context->setContext(context);
    m_context->setWidget(this);
    Core::ICore::addContextObject(m_context);

    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setMargin(0);
    toolbarLayout->setSpacing(0);
    m_toolbar->setLayout(toolbarLayout);

    toolbarLayout->addWidget(m_comboBox);

    m_closeButton->setIcon(QIcon(QLatin1String(Core::Constants::ICON_CLOSE_BUTTON)));
    m_closeButton->setToolTip(QCoreApplication::translate("Core", Core::Constants::TR_CLOSE));

    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(closeBrowsePane()));
    toolbarLayout->addWidget(m_closeButton);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    layout->addWidget(m_toolbar);
    layout->addWidget(m_stackedWidget);

    // setup combobox and stackedwidget
    {
        // device tree
        QAction *act = new QAction(tr("Activate Device View"), this);
        connect(act, SIGNAL(triggered()), this, SLOT(activateSubWidget()));

        Core::Command *cmd = Core::ActionManager::registerAction(act, Core::Id(Constants::MEDIASOURCE_DEVICE), context);
        cmd->setDefaultKeySequence(QKeySequence(tr("Alt+D")));

        m_actions.insert(IDX_DEVICE_WIDGET, act);
        m_commands.insert(IDX_DEVICE_WIDGET, cmd);
        m_comboBox->insertItem(IDX_DEVICE_WIDGET, tr("Devices"));
        m_stackedWidget->insertWidget(IDX_DEVICE_WIDGET, m_deviceWidget);
    }

    {
        // query
        QAction *act = new QAction(tr("Activate Query View"), this);
        connect(act, SIGNAL(triggered()), this, SLOT(activateSubWidget()));

        Core::Command *cmd = Core::ActionManager::registerAction(act, Core::Id(Constants::MEDIASOURCE_QUERY), context);
        cmd->setDefaultKeySequence(QKeySequence(tr("Alt+Q")));

        m_actions.insert(IDX_QUERY_WIDGET, act);
        m_commands.insert(IDX_QUERY_WIDGET, cmd);
        m_comboBox->insertItem(IDX_QUERY_WIDGET, tr("Query Records"));
        m_stackedWidget->insertWidget(IDX_QUERY_WIDGET, m_queryWidget);
    }

    connect(m_comboBox, SIGNAL(currentIndexChanged(int)), m_stackedWidget, SLOT(setCurrentIndex(int)));

    s_browsepanewidget = this;
}

BrowsePaneWidget::~BrowsePaneWidget()
{
    s_browsepanewidget = 0;

    Core::ICore::removeContextObject(m_context);
}

BrowsePaneWidget *BrowsePaneWidget::instance()
{
    return s_browsepanewidget;
}

void BrowsePaneWidget::setCurrentMediaSource(ProjectExplorer::MediaSource *currentMediaSource)
{
    // TODO: 需要处理 currentMediaSource 为空的情况
    if (!currentMediaSource) {
        Core::ICore::browsePane()->setWidget(0);
        return;
    }

    // update from factory
    IMediaSourceFactory *factory
            = MediaSourceFactoryManager::factory(currentMediaSource->classId());

    DeviceTree *dt = factory->deviceTree();
    s_browsepanewidget->m_deviceWidget->setBasicTree(dt);

    //
    s_browsepanewidget->m_queryWidget->setCurrentMediaSource(currentMediaSource);

    if (dt) {
        dt->setCurrentMediaSource(currentMediaSource);

        // TODO: cannot disconnect signals without a sender,
        // all we can do is to avoid duplicates.
        dt->disconnect(s_browsepanewidget);
        connect(dt, &DeviceTree::aboutToQueryCameras,
                s_browsepanewidget, []() { s_browsepanewidget->m_comboBox->setCurrentIndex(IDX_QUERY_WIDGET); });
    }
}

void BrowsePaneWidget::browseMediaSource(ProjectExplorer::MediaSource *mediasource)
{
    // TODO: when to set widget
    if (mediasource && mediasource->isValid() && mediasource->enabled())
        Core::ICore::browsePane()->setWidget(s_browsepanewidget);
}

Core::Command *BrowsePaneWidget::command(const QString &title) const
{
    int idx = m_comboBox->findText(title);
    if (idx != -1) {
        return m_commands.at(idx);
    }
    return 0;
}

void BrowsePaneWidget::activateSubWidget()
{
    QAction *act = qobject_cast<QAction *>(sender());
    int idx = m_actions.indexOf(act);
    m_comboBox->setCurrentIndex(idx);
}

void BrowsePaneWidget::closeBrowsePane()
{
    Core::ICore::browsePane()->setWidget(0);
}

} // namespace Internal
} // namespace MediaSource
