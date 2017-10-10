#include <QAction>
#include <QEvent>
#include <QMenu>
#include <QWidget>

#include "action/actioncontainer.h"
#include "action/actionmanager.h"
#include "action/command.h"
#include "icore.h"
#include "windowsupport.h"


namespace Core {
namespace Internal {

QList<QWidget *> WindowList::m_windows;
QList<QAction *> WindowList::m_windowActions;
QList<Id> WindowList::m_windowActionIds;

void WindowList::addWindow(QWidget *window)
{
    m_windows.append(window);
    Id id = Id("MediaStudio.Window.").withSuffix(m_windows.size());
    m_windowActionIds.append(id);
    auto action = new QAction(window->windowTitle(), 0);
    m_windowActions.append(action);
    QObject::connect(action, &QAction::triggered, [action]() { WindowList::activateWindow(action); });
    action->setCheckable(true);
    action->setChecked(false);
    Command *cmd = ActionManager::registerAction(action, id);
    cmd->setAttribute(Command::CA_UpdateText);
    ActionManager::actionContainer(Constants::M_WINDOW)->addAction(cmd, Constants::G_WINDOW_LIST);
    action->setVisible(window->isVisible() || window->isMinimized()); // minimized windows are hidden but should be shown
    QObject::connect(window, &QWidget::windowTitleChanged, [window]() { WindowList::updateTitle(window); });

    if (window->isActiveWindow())
        setActiveWindow(window);
}

void WindowList::removeWindow(QWidget *window)
{
    int index = m_windows.indexOf(window);

    if (index < 0) {
        Q_ASSERT(0);
        return;
    }
//    QTC_ASSERT(index >= 0, return);

    ActionManager::unregisterAction(m_windowActions.last(), m_windowActionIds.last());
    delete m_windowActions.takeLast();
    m_windowActionIds.removeLast();

    m_windows.removeOne(window);

    for (int i = index; i < m_windows.size(); ++i) {
        updateTitle(m_windows.at(i));
    }
}

void WindowList::setActiveWindow(QWidget *window)
{
    for (int i = 0; i < m_windows.size(); ++i) {
        m_windowActions.at(i)->setChecked(m_windows.at(i) == window);
    }
}

void WindowList::setWindowVisible(QWidget *window, bool visible)
{
    int index = m_windows.indexOf(window);
//    QTC_ASSERT(index >= 0, return);
//    QTC_ASSERT(index < m_windowActions.size(), return);
    m_windowActions.at(index)->setVisible(visible);
}

void WindowList::activateWindow(QAction *action)
{
    int index = m_windowActions.indexOf(action);
//    QTC_ASSERT(index >= 0, return);
//    QTC_ASSERT(index < m_windows.size(), return);
    ICore::raiseWindow(m_windows.at(index));
}

void WindowList::updateTitle(QWidget *window)
{
    int index = m_windows.indexOf(window);
//    QTC_ASSERT(index >= 0, return);
//    QTC_ASSERT(index < m_windowActions.size(), return);
    QString title = window->windowTitle();
    if (title.endsWith(QStringLiteral("- MediaStudio"))) {
        title.chop(13);
    }

    m_windowActions.at(index)->setText(title.trimmed());
}



WindowSupport::WindowSupport(QWidget *window, const Context &context)
    : QObject(window)
    , m_window(window)
{
    m_window->installEventFilter(this);

    m_contextObject = new IContext(this);
    m_contextObject->setWidget(window);
    m_contextObject->setContext(context);
    ICore::addContextObject(m_contextObject);

    m_toggleFullScreenAction = new QAction(this);
    updateFullScreenAction();
    ActionManager::registerAction(m_toggleFullScreenAction, Constants::TOGGLE_FULLSCREEN, context);
    connect(m_toggleFullScreenAction, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

    WindowList::addWindow(window);

    connect(ICore::instance(), &ICore::coreAboutToClose, this, [this]() { m_shutdown = true; });
}

WindowSupport::~WindowSupport()
{
    if (!m_shutdown) { // don't update all that stuff if we are shutting down anyhow
        ActionManager::unregisterAction(m_toggleFullScreenAction, Constants::TOGGLE_FULLSCREEN);
        ICore::removeContextObject(m_contextObject);
        WindowList::removeWindow(m_window);
    }
}

void WindowSupport::setCloseActionEnabled(bool enabled)
{
    Q_UNUSED(enabled);
}

bool WindowSupport::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != m_window)
        return false;

    if (event->type() == QEvent::WindowStateChange) {
        updateFullScreenAction();
    } else if (event->type() == QEvent::WindowActivate) {
        WindowList::setActiveWindow(m_window);
    } else if (event->type() == QEvent::Hide) {
        // minimized windows are hidden, but we still want to show them
        WindowList::setWindowVisible(m_window, m_window->isMinimized());
    } else if (event->type() == QEvent::Show) {
        WindowList::setWindowVisible(m_window, true);
    }

    return false;
}

void WindowSupport::toggleFullScreen()
{
    if (m_window->isFullScreen()) {
        m_window->setWindowState(m_window->windowState() & ~Qt::WindowFullScreen);
    } else {
        m_window->setWindowState(m_window->windowState() | Qt::WindowFullScreen);
    }
}

void WindowSupport::updateFullScreenAction()
{
    if (m_window->isFullScreen()) {
        m_toggleFullScreenAction->setChecked(true);
    } else {
        m_toggleFullScreenAction->setChecked(false);
    }
}


} // namespace Internal
} // namespace Core
