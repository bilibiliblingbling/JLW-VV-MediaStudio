#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLibraryInfo>
#include <QLoggingCategory>
#include <QNetworkProxyFactory>
#include <QSettings>
#include <QStandardPaths>
#include <QThreadPool>
#include <QTranslator>


#include <app/app_version.h>
#include <extensionsystem/iplugin.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>
#include <qtsingleapplication.h>



#define SHARE_PATH  "/../share/mediastudio"


enum { OptionIndent = 4, DescriptionIndent = 34 };
const char corePluginNameC[] = "Core";

const char HELP_OPTION1[] = "-h";
const char HELP_OPTION2[] = "-help";
const char HELP_OPTION3[] = "/h";
const char HELP_OPTION4[] = "--help";
const char VERSION_OPTION[] = "--version";
const char fixedOptionsC[] =
" [OPTION]... [FILE]...\n"
"Options:\n"
"    --help                         Display this help\n"
"    --version                      Display program version\n";

typedef QList<ExtensionSystem::PluginSpec *> PluginSpecSet;


static void displayHelpText(const QString &t)
{
    qWarning("%s", qPrintable(t));
}

static void displayError(const QString &t)
{
    qCritical("%s", qPrintable(t));
}

static void printVersion(const ExtensionSystem::PluginSpec *coreplugin)
{
    QString version;
    QTextStream str(&version);
    str << '\n' << Core::Constants::APP_NAME << ' ' << coreplugin->version() << " based on Qt " << qVersion() << "\n\n";
    ExtensionSystem::PluginManager::formatPluginVersions(str);
    str << '\n' << coreplugin->copyright() << '\n';
    displayHelpText(version);
}

static void printHelp(const QString &a0)
{
    QString help;
    QTextStream str(&help);
    str << "Usage: " << a0 << fixedOptionsC;
    ExtensionSystem::PluginManager::formatOptions(str, OptionIndent, DescriptionIndent);
    ExtensionSystem::PluginManager::formatPluginOptions(str, OptionIndent, DescriptionIndent);
    displayHelpText(help);
}

static inline QString msgCoreLoadFailure(const QString &why)
{
    return QCoreApplication::translate("Application", "Failed to load core: %1").arg(why);
}

static inline QStringList getPluginPaths()
{
    QStringList rc;
    // Figure out root:  Up one from 'bin'
    QDir rootDir = QApplication::applicationDirPath();
    rootDir.cdUp();
    const QString rootDirPath = rootDir.canonicalPath();

    // 1) "plugins" (Win/Linux)
    QString pluginPath = rootDirPath;
    pluginPath += QLatin1Char('/');
    pluginPath += QLatin1String(MEDIASTUDIO_LIBRARY_BASENAME);
    pluginPath += QLatin1String("/mediastudio/plugins");
    rc.push_back(pluginPath);

    // 3) <localappdata>/plugins/<ideversion>
    //    where <localappdata> is e.g.
    //    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
    //    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
    //    "~/Library/Application Support/QtProject/Qt Creator" on Mac
    pluginPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
            + QLatin1String("/data");
    pluginPath += QLatin1Char('/')
            + QLatin1String(Core::Constants::APP_ORGANIZATION_BRIEF)
            + QLatin1Char('/');
    pluginPath += QLatin1String("mediastudio");
    pluginPath += QLatin1String("/plugins/");
    rc.push_back(pluginPath);
    return rc;
}


int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("ms.*.debug=false");

    SharedTools::QtSingleApplication app((QLatin1String(Core::Constants::APP_NAME)), argc, argv);

    const int threadCnt = QThreadPool::globalInstance()->maxThreadCount();
    QThreadPool::globalInstance()->setMaxThreadCount(qMax(4, 2 * threadCnt));

    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope,
                       QCoreApplication::applicationDirPath() + QLatin1String(SHARE_PATH));
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings *settings =
            new QSettings(QSettings::IniFormat, QSettings::UserScope,
                          QLatin1String(Core::Constants::APP_ORGANIZATION_BRIEF),
                          QLatin1String(Core::Constants::APP_NAME));

    QSettings *globalSetings =
            new QSettings(QSettings::IniFormat, QSettings::SystemScope,
                          QLatin1String(Core::Constants::APP_ORGANIZATION_BRIEF),
                          QLatin1String(Core::Constants::APP_NAME));

    ExtensionSystem::PluginManager pluginManager;
    ExtensionSystem::PluginManager::setPluginIID(QLatin1String(Core::Constants::APP_PLUGIN_IID));
    ExtensionSystem::PluginManager::setGlobalSettings(globalSetings);
    ExtensionSystem::PluginManager::setSettings(settings);

    QTranslator translator;
    QTranslator qtTranslator;
    QStringList uiLanguages;

    uiLanguages = QLocale::system().uiLanguages();
    QString overrideLanguage = settings->value(QLatin1String("General/OverrideLanguage")).toString();
    if (!overrideLanguage.isEmpty()) {
        uiLanguages.prepend(overrideLanguage);
    }

    const QString &mediastudioTrPath = QCoreApplication::applicationDirPath()
           + QLatin1String(SHARE_PATH "/translations");

    foreach (QString locale, uiLanguages) {
        locale = QLocale(locale).name();
        if (translator.load(QLatin1String("mediastudio_") + locale, mediastudioTrPath)) {
            const QString &qtTrPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
            const QString &qtTrFile = QLatin1String("qt_") + locale;
            // Binary installer puts Qt tr files into mediastudioTrPath
            if (qtTranslator.load(qtTrFile, qtTrPath) || qtTranslator.load(qtTrFile, mediastudioTrPath)) {
                app.installTranslator(&translator);
                app.installTranslator(&qtTranslator);
                app.setProperty("mediastudio_locale", locale);
                break;
            }
            translator.load(QString()); // unload()
        } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
            // use built-in
            break;
        } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
            // use built-in
            break;
        }
    }

    // Make sure we honor the system's proxy settings
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    // Load
    const QStringList pluginPaths = getPluginPaths();
    ExtensionSystem::PluginManager::setPluginPaths(pluginPaths);

    QStringList arguments = app.arguments();
    QMap<QString, QString> foundAppOptions;
    if (arguments.empty()) {
        arguments << "-profile";
    }
    if (arguments.size() > 1) {
        QMap<QString, bool> appOptions;
        appOptions.insert(QLatin1String(HELP_OPTION1), false);
        appOptions.insert(QLatin1String(HELP_OPTION2), false);
        appOptions.insert(QLatin1String(HELP_OPTION3), false);
        appOptions.insert(QLatin1String(HELP_OPTION4), false);
        appOptions.insert(QLatin1String(VERSION_OPTION), false);
        QString errorMessage;
        if (!ExtensionSystem::PluginManager::parseOptions(arguments, appOptions, &foundAppOptions, &errorMessage)) {
            displayError(errorMessage);
            printHelp(QFileInfo(app.applicationFilePath()).baseName());
            return -1;
        }
    }

    const PluginSpecSet plugins = ExtensionSystem::PluginManager::plugins();
    ExtensionSystem::PluginSpec *coreplugin = NULL;
    foreach (ExtensionSystem::PluginSpec *spec, plugins) {
        if (spec->name() == QLatin1String(corePluginNameC)) {
            coreplugin = spec;
            break;
        }
    }
    if (!coreplugin) {
        QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1Char(',')));
        const QString reason = QCoreApplication::translate("Application", "Could not find Core plugin in %1").arg(nativePaths);
        displayError(msgCoreLoadFailure(reason));
        return 1;
    }
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }
    if (foundAppOptions.contains(QLatin1String(VERSION_OPTION))) {
        printVersion(coreplugin);
        return 0;
    }
    if (foundAppOptions.contains(QLatin1String(HELP_OPTION1))
            || foundAppOptions.contains(QLatin1String(HELP_OPTION2))
            || foundAppOptions.contains(QLatin1String(HELP_OPTION3))
            || foundAppOptions.contains(QLatin1String(HELP_OPTION4))) {
        printHelp(QFileInfo(app.applicationFilePath()).baseName());
        return 0;
    }

    ExtensionSystem::PluginManager::loadPlugins();
    if (coreplugin->hasError()) {
        displayError(msgCoreLoadFailure(coreplugin->errorString()));
        return 1;
    }

    // Set up remote arguments.
    QObject::connect(&app, SIGNAL(messageReceived(QString,QObject*)),
                     &pluginManager, SLOT(remoteArguments(QString,QObject*)));

    QObject::connect(&app, SIGNAL(fileOpenRequest(QString)),
                     coreplugin->plugin(), SLOT(fileOpenRequest(QString)));

    // shutdown plugin manager on the exit
    QObject::connect(&app, SIGNAL(aboutToQuit()), &pluginManager, SLOT(shutdown()));

    return app.exec();
}
