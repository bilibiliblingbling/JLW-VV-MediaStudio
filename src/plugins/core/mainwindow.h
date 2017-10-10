#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QList>
#include <QMap>

#include <utils/appmainwindow.h>

#include "icontext.h"
#include "icore.h"

QT_BEGIN_NAMESPACE
class QAction;
class QPrinter;
class QToolButton;
class QWidget;
QT_END_NAMESPACE

namespace Core {

class ICore;
class ModeManager;

class NavigationWidget;
class StatusBarWidget;

class SettingsDatabase;

namespace Internal {

class BrowsePane;
class MonitorPane;
class PreviewPane;
class TimelinePane;
class FancyTabWidget;
class WindowSupport;
class StatusBarManager;

class GeneralSettings;
class SystemSettings;
class ShortcutSettings;



class MainWindow : public Utils::AppMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    bool init(QString *errorMessage);
    void extensionsInitialized();
    void aboutToShutdown();

    IContext *contextObject(QWidget *widget);
    void addContextObject(IContext *context);
    void removeContextObject(IContext *context);

    inline SettingsDatabase *settingsDatabase() { return m_settingsDatabase; }
    virtual QPrinter *printer();

    QStatusBar *statusBar();

    Pane *browsePane();
    Pane *monitorPane();
    Pane *previewPane();
    Pane *timelinePane();


    IContext *currentContextObject() const;
    void updateAdditionalContexts(const Context &remove, const Context &add,
                                  ICore::ContextPriority priority);
    void setSuppressNavigationWidget(bool suppress);
    void setOverrideColor(const QColor &color);

signals:
    void newItemDialogRunningChanged();

public slots:
    void exit();
    void licenseChanged();
    bool showOptionsDialog(Id page = Id(), QWidget *parent = 0);


protected:
    void closeEvent(QCloseEvent *event);

private:
    void updateContextObject(const QList<IContext *> &context);
    void updateContext();

    void registerDefaultContainers();
    void registerDefaultActions();

    void readSettings();
    void writeSettings();

private slots:
    void setSidebarVisible(bool visible);
    void updateFocusWidget(QWidget *old, QWidget *now);
    void restoreWindowState();

    void manual();
    void aboutMediaStudio();

private:
    ICore *m_coreImpl;                      //!< 外部访问界面资源接口

    Context m_highPrioAdditionalContexts;   //!<
    Context m_lowPrioAdditionalContexts;    //!<

    ModeManager *m_modeManager;

    WindowSupport *m_windowSupport;         // 全屏, 最大, 最小化窗口
    FancyTabWidget *m_modeStack;            // central widget

    NavigationWidget *m_navigationWidget;   //!< 左侧导航栏
    BrowsePane *m_browsePane;               //!< 右上册浏览区域
    MonitorPane *m_monitorPane;             //!< 监视器显示区域
    PreviewPane *m_previewPane;             //!< 快速预览显示区域
    TimelinePane *m_timelinePane;           //!< 时间线显示区域

    StatusBarManager *m_statusBarManager;   //!< 底部状态栏

    mutable QPrinter *m_printer;

#if 0
    RightUpPaneWidget *m_rightUpPaneWidget; //!< 右上, 包括: 查询结果(偶尔) 及 监视器(长久)
    MonitorWidget *m_monitorWidget;         //!< 右上监视器
    PreviewWidget *m_previewWidet;          //!< 右中快速预览
    TimeLineWidget *m_timeLineWidget;       //!< 下侧时间线
    StatusBarWidget *m_statusBarWidget;
#endif

    QColor m_overrideColor;

    //
    SettingsDatabase *m_settingsDatabase;   //!< 配置数据库
    GeneralSettings *m_generalSettings;
    SystemSettings *m_systemSettigns;
    ShortcutSettings *m_shortcutSettings;

    //
    QList<IContext *> m_activeContext;
    QMap<QWidget *, IContext *> m_contextWidgets;

    //
    QAction *m_exitAction;
    QAction *m_optionsAction;

    QAction *m_toggleModeSelectorAction;
    QAction *m_toggleSideBarAction;
    QAction *m_toggleMonitorAction;
    QAction *m_togglePreviewAction;
    QAction *m_toggleTimelineAction;

    QToolButton *m_toggleSideBarButton;
};


} // namespace Internal
} // namespace Core

#endif // MAIN_WINDOW_H
