#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QPrinter>
#include <QStatusBar>
#include <QToolButton>


#include <app/app_version.h>
#include <core/icorelistener.h>
#include <extensionsystem/pluginmanager.h>
#include <utils/stylehelper.h>
#include <utils/tmplog.h>

#include "action/actioncontainer.h"
#include "action/actionmanager.h"
#include "action/actionmanager_p.h"
#include "action/command.h"

#include "browsepane.h"
#include "fancytabwidget.h"
#include "icore.h"
#include "inavigationwidgetfactory.h"
#include "mainwindow.h"
#include "manhattanstyle.h"
#include "modemanager.h"
#include "monitorpane.h"
#include "navigationwidget.h"
#include "previewpane.h"
#include "statusbarmanager.h"
#include "timelinepane.h"
#include "windowsupport.h"


namespace Core {
namespace Internal {

/*!

  +----+-------+-------+
  |    |       |       |
  | n  |       |       |
  | a  +---+---+---+---+
  | v  |   |   |   |   |
  | i  +---+---+---+---+
  |    |   |   |   |   |
  +----+---+---+---+---+
  |     time line      |
  +--------------------+

 */

MainWindow::MainWindow()
    : AppMainWindow()
    , m_coreImpl(new ICore(this))
    , m_lowPrioAdditionalContexts(Constants::C_GLOBAL)
    , m_windowSupport(0)
    , m_modeStack(new FancyTabWidget(this))
    , m_navigationWidget(0)
    , m_browsePane(0)
    , m_monitorPane(0)
    , m_previewPane(0)
    , m_timelinePane(0)
    , m_statusBarManager(0)
    , m_printer(0)
    , m_toggleModeSelectorAction(0)
    , m_toggleSideBarAction(0)
    , m_toggleMonitorAction(0)
    , m_togglePreviewAction(0)
    , m_toggleTimelineAction(0)
    , m_toggleSideBarButton(new QToolButton())
{
    setWindowTitle(Core::Constants::APP_NAME_FULL);

    QApplication::setWindowIcon(QIcon(QLatin1String(Core::Constants::ICON_MEDIASTUDIO)));

    QCoreApplication::setApplicationName(QLatin1String(Core::Constants::APP_NAME));
    QCoreApplication::setApplicationVersion(QLatin1String(Core::Constants::APP_VERSION_LONG));
    QCoreApplication::setOrganizationName(QLatin1String(Core::Constants::APP_ORGANIZATION_BRIEF));

    // TODO
    // theme settings
    QApplication::setStyle(new Core::ManhattanStyle("fusion"));

    setDockNestingEnabled(true);

    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);


    m_modeManager = new ModeManager(this, m_modeStack);

    registerDefaultContainers();
    registerDefaultActions();

    m_navigationWidget = new NavigationWidget(m_toggleSideBarAction);
    m_browsePane = new BrowsePane();
    m_monitorPane = new MonitorPane(m_toggleMonitorAction);
    m_previewPane = new PreviewPane(m_togglePreviewAction);
    m_timelinePane = new TimelinePane(m_toggleTimelineAction);

    m_statusBarManager = new StatusBarManager(this);


    setCentralWidget(m_modeStack);

    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(updateFocusWidget(QWidget*,QWidget*)));

    statusBar()->insertPermanentWidget(0, m_toggleSideBarButton);
    statusBar()->setProperty("p_styled", true);
}

MainWindow::~MainWindow()
{
    if (m_windowSupport) {
        delete m_windowSupport;
        m_windowSupport = 0;
    }

    if (m_printer) {
        delete m_printer;
        m_printer = 0;
    }

    if (m_statusBarManager) {
        delete m_statusBarManager;
        m_statusBarManager = 0;
    }

    if (m_coreImpl) {
        ExtensionSystem::PluginManager::removeObject(m_coreImpl);
        delete m_coreImpl;
        m_coreImpl = 0;
    }

    if (m_modeManager) {
        delete m_modeManager;
        m_modeManager = 0;
    }
}

bool MainWindow::init(QString *errorMessage)
{
    Q_UNUSED(errorMessage);

    ExtensionSystem::PluginManager::addObject(m_coreImpl);
    m_statusBarManager->init();
    m_modeManager->init();

    return true;
}

void MainWindow::extensionsInitialized()
{
    // TODO

    m_windowSupport = new WindowSupport(this, Context("Core.MainWindow"));
    m_windowSupport->setCloseActionEnabled(false);
    m_statusBarManager->extensionsInitalized();
    m_navigationWidget->setFactories(ExtensionSystem::PluginManager::getObjects<INavigationWidgetFactory>());

    readSettings();
    updateContext();

    emit m_coreImpl->coreAboutToOpen();

    QTimer::singleShot(0, this, SLOT(restoreWindowState()));
    QTimer::singleShot(0, m_coreImpl, SIGNAL(coreOpened()));

}

void MainWindow::aboutToShutdown()
{
    // TODO
    hide();
}

IContext *MainWindow::contextObject(QWidget *widget)
{
    return m_contextWidgets.value(widget);
}

void MainWindow::addContextObject(IContext *context)
{
    if (!context)
        return;

    QWidget *widget = context->widget();
    if (m_contextWidgets.contains(widget))
        return;

    m_contextWidgets.insert(widget, context);
}

void MainWindow::removeContextObject(IContext *context)
{
    if (!context)
        return;

    QWidget *widget = context->widget();
    if (!m_contextWidgets.contains(widget))
        return;

    m_contextWidgets.remove(widget);
    if (m_activeContext.removeAll(context) > 0)
        updateContextObject(m_activeContext);
}

void MainWindow::setOverrideColor(const QColor &color)
{
    m_overrideColor = color;
}

QPrinter *MainWindow::printer()
{
    if (!m_printer) {
        m_printer = new QPrinter(QPrinter::HighResolution);
    }

    return m_printer;
}

QStatusBar *MainWindow::statusBar()
{
    return m_modeStack->statusBar();
}

Pane *MainWindow::browsePane()
{
    return m_browsePane;
}

Pane *MainWindow::monitorPane()
{
    return m_monitorPane;
}

Pane *MainWindow::previewPane()
{
    return m_previewPane;
}

Pane *MainWindow::timelinePane()
{
    return m_timelinePane;
}

IContext *MainWindow::currentContextObject() const
{
    return (m_activeContext.isEmpty()? 0: m_activeContext.first());
}

void MainWindow::updateAdditionalContexts(const Context &remove, const Context &add,
                                          ICore::ContextPriority priority)
{
    foreach (const Id id, remove) {
        if (!id.isValid())
            continue;

        int index = m_lowPrioAdditionalContexts.indexOf(id);
        if (index != -1)
            m_lowPrioAdditionalContexts.removeAt(index);

        index = m_highPrioAdditionalContexts.indexOf(id);
        if (index != -1)
            m_highPrioAdditionalContexts.removeAt(index);
    }

    foreach (const Id id, add) {
        if (!id.isValid())
            continue;

        Context &cref = (priority == ICore::ContextPriority::High ? m_highPrioAdditionalContexts
                                                                  : m_lowPrioAdditionalContexts);
        if (!cref.contains(id))
            cref.prepend(id);
    }

    updateContext();
}

void MainWindow::setSuppressNavigationWidget(bool suppress)
{
    if (NavigationWidgetPlaceHolder::current()) {
        m_navigationWidget->setSuppressed(suppress);
    }
}

void MainWindow::exit()
{
    QTimer::singleShot(0, this,  SLOT(close()));
}

void MainWindow::licenseChanged()
{
    // respect the license
}

bool MainWindow::showOptionsDialog(Id page, QWidget *parent)
{
    emit m_coreImpl->optionsDialogRequested();

    if (!parent) {
        parent = ICore::dialogParent();
    }

//    SettingsDialog *dialog = SettingsDialog::getSettingsDialog(parent, page);
//    return dialog->execDialog();
    // TODO
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ICore::saveSettings();

    const QList<ICoreListener *> listeners =
        ExtensionSystem::PluginManager::getObjects<ICoreListener>();

    foreach (ICoreListener *listener, listeners) {
        if (!listener->coreAboutToClose()) {
            event->ignore();
            return;
        }
    }

    emit m_coreImpl->coreAboutToClose();

    writeSettings();

    m_navigationWidget->closeSubWidgets();
}

void MainWindow::updateContextObject(const QList<IContext *> &context)
{
    emit m_coreImpl->contextAboutToChange(context);
    m_activeContext = context;
    updateContext();
}

void MainWindow::updateContext()
{
    Context contexts = m_highPrioAdditionalContexts;

    foreach (IContext *context, m_activeContext) {
        contexts.add(context->context());
    }

    contexts.add(m_lowPrioAdditionalContexts);

    Context uniquecontexts;
    for (int i = 0; i < contexts.size(); ++i) {
        const Id id = contexts.at(i);
        if (!uniquecontexts.contains(id))
            uniquecontexts.add(id);
    }

    ActionManager::setContext(uniquecontexts);
    emit m_coreImpl->contextChanged(uniquecontexts);
}

void MainWindow::registerDefaultContainers()
{
    // Menu Bar
    ActionContainer *menubar = ActionManager::createMenuBar(Constants::MENU_BAR);
    setMenuBar(menubar->menuBar());

    menubar->appendGroup(Constants::G_FILE);
    menubar->appendGroup(Constants::G_WINDOW);
    menubar->appendGroup(Constants::G_TOOLS);
    menubar->appendGroup(Constants::G_HELP);

    // File Menu
    ActionContainer *filemenu = ActionManager::createMenu(Constants::M_FILE);
    menubar->addMenu(filemenu, Constants::G_FILE);
    filemenu->menu()->setTitle(tr("&File"));
    filemenu->appendGroup(Constants::G_FILE_PROJECT);
    filemenu->appendGroup(Constants::G_FILE_OTHTER);

    // Window Menu
    ActionContainer *windowmenu = ActionManager::createMenu(Constants::M_WINDOW);
    menubar->addMenu(windowmenu, Constants::G_WINDOW);
    windowmenu->appendGroup(Constants::G_WINDOW_VIEWS);
    windowmenu->appendGroup(Constants::G_WINDOW_LIST);
    windowmenu->menu()->setTitle(tr("&Window"));

    // Tools Menu
    ActionContainer *toolsmenu = ActionManager::createMenu(Constants::M_TOOLS);
    menubar->addMenu(toolsmenu, Constants::G_TOOLS);
    toolsmenu->appendGroup(Constants::G_TOOLS_OPTIONS);
    toolsmenu->menu()->setTitle(tr("&Tools"));

    // Help Menu
    ActionContainer *helpmenu = ActionManager::createMenu(Constants::M_HELP);
    menubar->addMenu(helpmenu, Constants::G_HELP);
    helpmenu->appendGroup(Constants::G_HELP_MANUAL);
    helpmenu->appendGroup(Constants::G_HELP_ABOUT);
    helpmenu->menu()->setTitle(tr("&Help"));
}

void MainWindow::registerDefaultActions()
{
    Command *cmd = 0;
    QAction *tmpact = 0;


    // File Menu
    ActionContainer *filemenu = ActionManager::actionContainer(Constants::M_FILE);
    filemenu->addSeparator(Constants::G_FILE_PROJECT);
    filemenu->addSeparator(Constants::G_FILE_OTHTER);

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setMenuRole(QAction::QuitRole);
    cmd = ActionManager::registerAction(m_exitAction, Constants::EXIT);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Q")));
    filemenu->addAction(cmd, Constants::G_FILE_OTHTER);
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(exit()));


    // Window Menu
    ActionContainer *windowmenu = ActionManager::actionContainer(Constants::M_WINDOW);
    windowmenu->addSeparator(Constants::G_WINDOW_VIEWS);

    // TODO: update action text dynamically
    m_toggleModeSelectorAction = new QAction(tr("Show Mode Selector"), this);
    m_toggleModeSelectorAction->setCheckable(true);
    cmd = ActionManager::registerAction(m_toggleModeSelectorAction, Constants::TOGGLE_MODE_SELECTPR);
    connect(m_toggleModeSelectorAction, &QAction::triggered,
            ModeManager::instance(), &ModeManager::setModeSelectorVisible);
    windowmenu->addAction(cmd, Constants::G_WINDOW_VIEWS);

    m_toggleSideBarAction = new QAction(QIcon(QLatin1String(Constants::ICON_TOGGLE_SIDEBAR)),
                                        QCoreApplication::translate("Core", Constants::TR_SHOW_SIDEBAR), this);
    m_toggleSideBarAction->setCheckable(true);
    cmd = ActionManager::registerAction(m_toggleSideBarAction, Constants::TOGGLE_SIDEBAR);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDefaultKeySequence(QKeySequence(tr("Alt+0")));
    connect(m_toggleSideBarAction, SIGNAL(triggered(bool)), this, SLOT(setSidebarVisible(bool)));
    m_toggleSideBarButton->setDefaultAction(cmd->action());
    windowmenu->addAction(cmd, Constants::G_WINDOW_VIEWS);
    m_toggleSideBarAction->setEnabled(false);

    m_toggleMonitorAction = new QAction(QCoreApplication::translate(Constants::CORE_TR_SCOPE, Constants::TR_SHOW_MONITOR), this);
    m_toggleMonitorAction->setCheckable(true);
    cmd = ActionManager::registerAction(m_toggleMonitorAction, Constants::TOGGLE_MONITOR);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDefaultKeySequence(QKeySequence(tr("Alt+1")));
    windowmenu->addAction(cmd, Constants::G_WINDOW_VIEWS);
    m_toggleMonitorAction->setEnabled(false);

    m_togglePreviewAction = new QAction(QCoreApplication::translate(Constants::CORE_TR_SCOPE, Constants::TR_SHOW_PREVIEW), this);
    m_togglePreviewAction->setCheckable(true);
    cmd = ActionManager::registerAction(m_togglePreviewAction, Constants::TOGGLE_PREVIEW);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDefaultKeySequence(QKeySequence(tr("Alt+2")));
    windowmenu->addAction(cmd, Constants::G_WINDOW_VIEWS);
    m_togglePreviewAction->setEnabled(false);

    m_toggleTimelineAction = new QAction(QCoreApplication::translate(Constants::CORE_TR_SCOPE, Constants::TR_SHOW_TIMELINE), this);
    m_toggleTimelineAction->setCheckable(true);
    cmd = ActionManager::registerAction(m_toggleTimelineAction, Constants::TOGGLE_TIMELINE);
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setDefaultKeySequence(QKeySequence(tr("Alt+3")));
    windowmenu->addAction(cmd, Constants::G_WINDOW_VIEWS);
    m_toggleTimelineAction->setEnabled(false);

    // Tools Menu
    ActionContainer *toolsmenu = ActionManager::actionContainer(Constants::M_TOOLS);
    toolsmenu->addSeparator(Constants::G_TOOLS_OPTIONS);

    tmpact = new QAction(tr("&Options..."), this);
    tmpact->setMenuRole(QAction::PreferencesRole);
    cmd = ActionManager::registerAction(tmpact, Constants::OPTIONS);
    toolsmenu->addAction(cmd, Constants::G_TOOLS_OPTIONS);
    connect(tmpact, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));


    // Help Memu
    ActionContainer *helpmenu = ActionManager::actionContainer(Constants::M_HELP);
    helpmenu->addSeparator(Constants::G_HELP_ABOUT);

    tmpact = new QAction(tr("&Manual"), this);
    tmpact->setMenuRole(QAction::ApplicationSpecificRole);
    cmd = ActionManager::registerAction(tmpact, Constants::MANUAL);
    helpmenu->addAction(cmd, Constants::G_HELP_MANUAL);
    tmpact->setEnabled(true);
    connect(tmpact, &QAction::triggered, this, &MainWindow::manual);

    tmpact = new QAction(tr("About Media&Studio..."), this);
    tmpact->setMenuRole(QAction::AboutRole);
    cmd = ActionManager::registerAction(tmpact, Constants::ABOUT);
    helpmenu->addAction(cmd, Constants::G_HELP_ABOUT);
    connect(tmpact, &QAction::triggered, this, &MainWindow::aboutMediaStudio);
}

static const char settingsGroup[] = "MainWindow";
static const char colorKey[] = "Color";
static const char windowGeometryKey[] = "WindowGeometry";
static const char windowStateKey[] = "WindowState";
static const char modeSelectorVisibleKey[] = "ModeSelectorVisible";

void MainWindow::readSettings()
{
    QSettings *settings = ExtensionSystem::PluginManager::settings();

    settings->beginGroup(settingsGroup);

    if (m_overrideColor.isValid()) {
        Utils::StyleHelper::setBaseColor(m_overrideColor);
        m_overrideColor = Utils::StyleHelper::baseColor();
    } else {
        Utils::StyleHelper::setBaseColor(
                    settings->value(QLatin1String(colorKey),
                                    QColor(Utils::StyleHelper::DEFAULT_BASE_COLOR)).value<QColor>());
    }
    bool modeSelectorVisible = settings->value(QLatin1String(modeSelectorVisibleKey), true).toBool();
    ModeManager::setModeSelectorVisible(modeSelectorVisible);
    m_toggleModeSelectorAction->setChecked(modeSelectorVisible);

    settings->endGroup();

    m_navigationWidget->restoreSettings(settings);
    m_browsePane->restoreSettings(settings);
    m_monitorPane->restoreSettings(settings);
    m_previewPane->restoreSettings(settings);
    m_timelinePane->restoreSettings(settings);
}

void MainWindow::writeSettings()
{
    QSettings *settings = ExtensionSystem::PluginManager::settings();

    settings->beginGroup(QLatin1String(settingsGroup));

    if (!(m_overrideColor.isValid() && (Utils::StyleHelper::baseColor() == m_overrideColor))) {
        settings->setValue(QLatin1String(colorKey), Utils::StyleHelper::requestedBaseColor());
    }

    settings->setValue(QLatin1String(windowGeometryKey), saveGeometry());
    settings->setValue(QLatin1String(windowStateKey), saveState());
    settings->setValue(QLatin1String(modeSelectorVisibleKey), ModeManager::isModeSelectorVisible());

    settings->endGroup();

    ActionManager::saveSettings();
    m_navigationWidget->saveSettings(settings);
    m_browsePane->saveSettings(settings);
    m_monitorPane->saveSettings(settings);
    m_previewPane->saveSettings(settings);
    m_timelinePane->saveSettings(settings);
}

void MainWindow::setSidebarVisible(bool visible)
{
    if (NavigationWidgetPlaceHolder::current()) {
        if (m_navigationWidget->isSuppressed() && visible) {
            m_navigationWidget->setShown(true);
            m_navigationWidget->setSuppressed(false);
        } else {
            m_navigationWidget->setShown(visible);
        }
    }
}

void MainWindow::updateFocusWidget(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    // Prevent changing the context object just because the menu or a menu item is activated
    if (qobject_cast<QMenuBar*>(now) || qobject_cast<QMenu*>(now))
        return;

    QList<IContext *> newContext;
    if (QWidget *p = qApp->focusWidget()) {
        IContext *context = 0;
        while (p) {
            context = m_contextWidgets.value(p);
            if (context)
                newContext.append(context);
            p = p->parentWidget();
        }
    }

    // ignore toplevels that define no context, like popups without parent
    if (!newContext.isEmpty() || qApp->focusWidget() == focusWidget())
        updateContextObject(newContext);
}

void MainWindow::restoreWindowState()
{
    QSettings *settings = ExtensionSystem::PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));
    if (!restoreGeometry(settings->value(QLatin1String(windowGeometryKey)).toByteArray())) {
        resize(1008, 700); // size without window decoration
    }
    restoreState(settings->value(QLatin1String(windowStateKey)).toByteArray());
    settings->endGroup();
    show();

    m_statusBarManager->restoreSettings();
}

void MainWindow::manual()
{
    TM_MARK;
}

void MainWindow::aboutMediaStudio()
{
    TM_MARK;
}

} // namespace Internal
} // namespace Core
