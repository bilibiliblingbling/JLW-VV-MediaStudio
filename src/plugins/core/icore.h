#ifndef ICORE_H
#define ICORE_H

#include <QObject>
#include <QSettings>

#include "core_global.h"
#include "id.h"

QT_BEGIN_NAMESPACE
class QPrinter;
class QStatusBar;
class QWidget;
template <class T> class QList;
QT_END_NAMESPACE

namespace Core {
class IWizardFactory;
class Context;
class IContext;
class Pane;
class ProgressManager;
class SettingsDatabase;
namespace Internal { class MainWindow; }

class CORE_EXPORT ICore : public QObject
{
    Q_OBJECT

    friend class Internal::MainWindow;
    friend class IWizardFactory;

    explicit ICore(Internal::MainWindow *mw);
    ~ICore();

public:
    enum class ContextPriority { High, Low };

    // This should only be used to acccess the signals, so it could
    // theoretically return an QObject *. For source compatibility
    // it returns a ICore.
    static ICore *instance();

    static bool isNewItemDialogRunning();
    static void showNewItemDialog(const QString &title,
                                  const QList<IWizardFactory *> &factories,
                                  const QString &defaultLocation = QString(),
                                  const QVariantMap &extraVariables = QVariantMap());

    static bool showOptionsDialog(Id page, QWidget *parent = 0);
    static QString msgShowOptionsDialog();
    static QString msgShowOptionsDialogToolTip();

    static bool showWarningWithOptions(const QString &title, const QString &text,
                                       const QString &details = QString(),
                                       Id settingsId = Id(),
                                       QWidget *parent = 0);

    static QSettings *settings(QSettings::Scope scope = QSettings::UserScope);
    static SettingsDatabase *settingsDatabase();
    static QPrinter *printer();
    static QString userInterfaceLanguage();

    static QString resourcePath();
    static QString userResourcePath();
    static QString documentationPath();
    static QString libexecPath();

    static QString versionString();
    static QString buildCompatibilityString();

    static QWidget *mainWindow();
    static QWidget *dialogParent();
    static QStatusBar *statusBar();

    static Pane *browsePane();
    static Pane *monitorPane();
    static Pane *previewPane();
    static Pane *timelinePane();

    /* Raises and activates the window for the widget. This contains workarounds for X11. */
    static void raiseWindow(QWidget *widget);

    static IContext *currentContextObject();
    // Adds and removes additional active contexts, these contexts are appended
    // to the currently active contexts.
    static void updateAdditionalContexts(const Context &remove, const Context &add,
                                         ContextPriority priority = ContextPriority::Low);
    static void addAdditionalContext(const Context &context,
                                     ContextPriority priority = ContextPriority::Low);
    static void removeAdditionalContext(const Context &context);
    static void addContextObject(IContext *context);
    static void removeContextObject(IContext *context);

    // manages the minimize, zoom and fullscreen actions for the window
    static void registerWindow(QWidget *window, const Context &context);

    enum OpenFilesFlags {
        None = 0,
        SwitchMode = 1,
        CanContainLineAndColumnNumbers = 2,
         /// Stop loading once the first file fails to load
        StopOnLoadFail = 4
    };
    static void openFiles(const QStringList &fileNames, OpenFilesFlags flags = None);

public slots:
    static void saveSettings();

signals:
    void coreAboutToOpen();
    void coreOpened();
    void newItemDialogRunningChanged();
    void saveSettingsRequested();
    void optionsDialogRequested();
    void coreAboutToClose();
    void contextAboutToChange(const QList<Core::IContext *> &context);
    void contextChanged(const Core::Context &context);
    void themeChanged();

private:
    static void validateNewItemDialogIsRunning();
    static void newItemDialogOpened();
    static void newItemDialogClosed();
};

} // namespace Core

#endif // ICORE_H
