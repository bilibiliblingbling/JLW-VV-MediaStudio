#include <QApplication>
#include <QDateTime>
#include <utils/tmplog.h>

#include <extensionsystem/pluginmanager.h>
#include <utils/savefile.h>
#include <utils/theme/theme.h>

#include "action/actionmanager.h"
#include "coreplugin.h"
#include "editmode.h"
#include "icore.h"
#include "mainwindow.h"
#include "modemanager.h"


namespace Core {
namespace Internal {

CorePlugin::CorePlugin()
    : m_mainWindow(0)
    , m_editMode(0)
{
    qRegisterMetaType<Id>();
}

CorePlugin::~CorePlugin()
{
    if (m_editMode) {
        removeObject(m_editMode);
        delete m_editMode;
        m_editMode = 0;
    }

    if (m_mainWindow) {
        delete m_mainWindow;
        m_mainWindow = 0;
    }
    Utils::setMediaStudioTheme(0);
}

bool CorePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    new ActionManager(this);
    qsrand(QDateTime::currentDateTime().toTime_t());

    parseArguments(arguments);

    const bool success = m_mainWindow->init(errorString);
    if (success) {
        m_editMode = new EditMode();
        addObject(m_editMode);
        ModeManager::activateMode(m_editMode->id());
    }

    Utils::SaveFile::initializeUmask();

    return success;
}

void CorePlugin::extensionsInitialized()
{
    m_mainWindow->extensionsInitialized();

    if (ExtensionSystem::PluginManager::hasError()) {
        TM_DBG("has error");
    }
}

bool CorePlugin::delayedInitialize()
{
    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag CorePlugin::aboutToShutdown()
{
    m_mainWindow->aboutToShutdown();
    return SynchronousShutdown;
}

QObject *CorePlugin::remoteCommand(const QStringList &, const QString &, const QStringList &)
{
    TM_MARK;
    m_mainWindow->activateWindow();

    return 0;
}

void CorePlugin::fileOpenRequest(const QString &f)
{
    TM_MARK;
    remoteCommand(QStringList(), QString(), QStringList(f));
}

void CorePlugin::parseArguments(const QStringList &arguments)
{
    // TODO
    Q_UNUSED(arguments);

    Utils::Theme::initialPalette();

    const QString defaultTheme = QLatin1String("default");
    QString themeName = ICore::settings()->value(
                QLatin1String(Constants::SETTINGS_THEME), defaultTheme).toString();
    QColor overrideColor;
    bool presentationMode = false;

    QString themeURI;
    if (themeURI.isEmpty()) {
        themeName = defaultTheme;
        themeURI = QStringLiteral("%1/themes/%2.creatortheme").arg(ICore::resourcePath()).arg(themeName);
        if (themeURI.isEmpty()) {
            qCritical("%s", qPrintable(QCoreApplication::translate("Application", "No valid theme \"%1\"")
                                       .arg(themeName)));
        }
    }

    QSettings themeSettings(themeURI, QSettings::IniFormat);
    Utils::Theme *theme = new Utils::Theme(themeName, qApp);
    theme->readSettings(themeSettings);
    if (theme->flag(Utils::Theme::ApplyThemePaletteGlobally))
        QApplication::setPalette(theme->palette());
    Utils::setMediaStudioTheme(theme);

    // defer creation of these widgets until here,
    // because they need a valid theme set
    m_mainWindow = new MainWindow();
    ActionManager::setPresentationModeEnabled(presentationMode);

    if (overrideColor.isValid())
        m_mainWindow->setOverrideColor(overrideColor);
}


} // namespace Internal
} // namespace Core
