#ifndef WINDOW_SUPPORT_H
#define WINDOW_SUPPORT_H

#include <QObject>

#include "icontext.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class WindowList
{
public:
    static void addWindow(QWidget *window);
    static void removeWindow(QWidget *window);
    static void setActiveWindow(QWidget *window);
    static void setWindowVisible(QWidget *window, bool visible);

private:
    static void activateWindow(QAction *action);
    static void updateTitle(QWidget *window);

private:
    static QList<QWidget *> m_windows;
    static QList<QAction *> m_windowActions;
    static QList<Id> m_windowActionIds;
};


class WindowSupport : public QObject
{
    Q_OBJECT

public:
    WindowSupport(QWidget *window, const Context &context);
    ~WindowSupport();

    void setCloseActionEnabled(bool enabled);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void toggleFullScreen();
    void updateFullScreenAction();

private:
    QWidget *m_window;
    IContext *m_contextObject;
    QAction *m_minimizeAction;
    QAction *m_zoomAction;
    QAction *m_closeAction;
    QAction *m_toggleFullScreenAction;
    bool m_shutdown = false;
};



} // namespace Internal
} // namespace Core

#endif // WINDOW_SUPPORT_H
