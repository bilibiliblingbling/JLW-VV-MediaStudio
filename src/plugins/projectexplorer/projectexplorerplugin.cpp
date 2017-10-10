#include <QAction>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>

#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>
#include <core/icore.h>

#include <extensionsystem/pluginmanager.h>
#include <utils/mimedatabase.h>
#include <utils/stringutils.h>
#include <utils/tmplog.h>

#include "basictreemanager.h"
#include "constants.h"
#include "corelistener.h"
#include "iprojectmanager.h"
#include "mediasourcetimer.h"
#include "project.h"
#include "projectexplorerplugin.h"
#include "projecttreewidget.h"
#include "segrepotreewidget.h"
#include "sessionmanager.h"


namespace ProjectExplorer {
namespace Internal {
class ProjectExplorerPluginPrivate : public QObject
{
    Q_DECLARE_TR_FUNCTIONS(ProjectExplorer::ProjectExplorerPlugin)

public:
    ProjectExplorerPluginPrivate();
    ~ProjectExplorerPluginPrivate();

public:
    void newProject();
    void openProject(QString filename = QString());
    void updateRecentProjectMenu();
    void clearRecentProjects();

public:
    void savePersistentSettings();
    void loadPersistentSettings();

public:
    void addToRecentProjects(const QString &filename, const QString &displayName);

public:
    QAction *m_newAction;
    QAction *m_openAction;

public:
    static const quint8 m_maxRecentProjects = 25;
    QList<QPair<QString, QString> > m_recentProjects;
    QString m_lastOpenDirectory;
};

const char RECENTPROJECTS_FILENAMES[]       = "ProjectExplorer/RecentProjects/FileNames";
const char RECENTPROJECTS_DISPLAYNAMES[]    = "ProjectExplorer/RecentProjects/DisplayNames";

ProjectExplorerPluginPrivate::ProjectExplorerPluginPrivate()
{
}

ProjectExplorerPluginPrivate::~ProjectExplorerPluginPrivate()
{
}

void ProjectExplorerPluginPrivate::newProject()
{
    TM_MARK;
    // TODO
}

void ProjectExplorerPluginPrivate::openProject(QString filename)
{
    TM_MARK;
    // TODO

    if (filename.isEmpty()) {
        const QString projectFilters = Utils::MimeDatabase::allFiltersString();

        filename = QFileDialog::getOpenFileName(Core::ICore::dialogParent(),
                                                tr("Open Project"),
                                                m_lastOpenDirectory,
                                                projectFilters);
    }

    if (filename.isEmpty())
        return;

    TM_DBG << filename.toStdString();

    QString errorString;
    ProjectExplorerPlugin::openProject(filename, &errorString);
    if (!errorString.isEmpty()) {
        QMessageBox::critical(Core::ICore::mainWindow(),
                              tr("Failed to open project."),
                              errorString);
    }
}

void ProjectExplorerPluginPrivate::updateRecentProjectMenu()
{
    Core::ActionContainer *ac = Core::ActionManager::actionContainer(Constants::M_ROJECT_RECENTPROJECTS);
    QMenu *menu = ac->menu();
    menu->clear();

    bool hasRecentProjects = !m_recentProjects.isEmpty();
    QList<QPair<QString, QString> >::const_iterator cit = m_recentProjects.begin();
    for (; cit != m_recentProjects.end(); ++cit) {
        const QString filename = cit->first;
        QAction *act = menu->addAction(Utils::withTildeHomePath(filename));
        connect(act, &QAction::triggered,
                this, [this, filename]() { openProject(filename); });
    }
    menu->setEnabled(hasRecentProjects);

    if (hasRecentProjects) {
        menu->addSeparator();
        QAction *act = menu->addAction(QCoreApplication::translate("Core", Core::Constants::TR_CLEAR_MENU));
        connect(act, &QAction::triggered,
                this, &ProjectExplorerPluginPrivate::clearRecentProjects);
    }
    emit ProjectExplorerPlugin::instance()->recentProjectsChanged();
    // TODO: set recent opened projects
}

void ProjectExplorerPluginPrivate::clearRecentProjects()
{
    m_recentProjects.clear();
    // TODO: update welcome page
}

void ProjectExplorerPluginPrivate::savePersistentSettings()
{
    // TODO: dd->shuttingdown

    QSettings *s = Core::ICore::settings();

    QStringList fileNames;
    QStringList displayNames;
    QList<QPair<QString, QString> >::const_iterator cit = m_recentProjects.begin();
    for (; cit != m_recentProjects.end(); ++cit) {
        fileNames << cit->first;
        displayNames << cit->second;
    }

    s->setValue(QLatin1String(RECENTPROJECTS_FILENAMES), fileNames);
    s->setValue(QLatin1String(RECENTPROJECTS_DISPLAYNAMES), displayNames);
}

void ProjectExplorerPluginPrivate::loadPersistentSettings()
{
    QSettings *s = Core::ICore::settings();
    const QStringList fileNames =
            s->value(QLatin1String(RECENTPROJECTS_FILENAMES)).toStringList();
    const QStringList displayNames =
            s->value(QLatin1String(RECENTPROJECTS_DISPLAYNAMES)).toStringList();

    if (fileNames.size() == displayNames.size()) {
        for (int i = 0; i < fileNames.size(); ++i) {
            if (QFileInfo(fileNames.at(i)).isFile()) {
                m_recentProjects.append(qMakePair(fileNames.at(i), displayNames.at(i)));
            }
        }
    }
}

void ProjectExplorerPluginPrivate::addToRecentProjects(const QString &filename, const QString &displayName)
{
    if (filename.isEmpty())
        return;

    QString prettyFileName = QDir::toNativeSeparators(filename);

    QList<QPair<QString, QString> >::iterator it = m_recentProjects.begin();
    while (it != m_recentProjects.end()) {
        if (it->first == prettyFileName)
            it = m_recentProjects.erase(it);
        else
            ++it;
    }

    if (m_recentProjects.count() > m_maxRecentProjects)
        m_recentProjects.removeLast();

    m_recentProjects.prepend(qMakePair(prettyFileName, displayName));
    QFileInfo fi(prettyFileName);
#if 0
    m_lastOpenDirectory = fi.absolutePath();
#else
    m_lastOpenDirectory = prettyFileName;
#endif

    emit ProjectExplorerPlugin::instance()->recentProjectsChanged();
}

} // namespace Internal

/*!
    所有数据:
     1. 元数据: 媒体源, 媒体库/媒体片段, 时间线, 输出设备(使用插件支持多 `输出设备' 的接入)
     2. 配置信息: 时间线配置, 快速预览配置, 监视器配置,
    [x] 摄像机列表等, 不归工程文件处理, 而是由各源自由定义功能, 并统一注册到 browse pane 进行显示
 */

/*!
    \class ProjectExplorerPlugin

    \brief The ProjectExplorerPlugin class

    工程文件的解析, 生成可供其他模块使用的数据
 */


static ProjectExplorerPlugin *s_instance = 0;
static Internal::ProjectExplorerPluginPrivate *dd = 0;

ProjectExplorerPlugin::ProjectExplorerPlugin()
{
    setObjectName(QLatin1String("ProjectExplorer.ProjectExplorerPlugin"));

    dd = new Internal::ProjectExplorerPluginPrivate();

    s_instance = this;
}

ProjectExplorerPlugin::~ProjectExplorerPlugin()
{
    removeObject(this);

    s_instance = 0;
    delete dd;
}

ProjectExplorerPlugin *ProjectExplorerPlugin::instance()
{
    return s_instance;
}

Project *ProjectExplorerPlugin::openProject(const QString &fileName, QString *errorString)
{
    QList<Project *> lst = openProjects(QStringList() << fileName, errorString);

    if (lst.isEmpty())
        return 0;

    dd->addToRecentProjects(fileName, lst.first()->displayName());

    SessionManager::setStartupProject(lst.first());

    return lst.first();
}

static void appendError(QString *errorString, const QString &error)
{
    if (!errorString || error.isEmpty())
        return;

    if (!errorString->isEmpty())
        errorString->append(QLatin1Char('\n'));

    errorString->append(error);
}

QList<Project *> ProjectExplorerPlugin::openProjects(const QStringList &fileNames, QString *errorString)
{
    QList<Project *> projects;

    // TODO: more info required

    const QList<IProjectManager *> projectManagers =
            ExtensionSystem::PluginManager::getObjects<IProjectManager>();

    QList<Project *> alreadyOpened;
    foreach (const QString &fileName, fileNames) {
        // check whether already opened
        const QFileInfo fi(fileName);
        const QString filePath = fi.absoluteFilePath();
        bool found = false;
        foreach (Project *pi, SessionManager::projects()) {
            if (filePath == pi->projectFilePath()) {
                alreadyOpened.append(pi);
                found = true;
                break;
            }
        }
        if (found) {
            TM_DBG << fileName.toStdString() << "already opened";
//            SessionManager::reportProjectLoadingProgress();
            continue;
        }

        Utils::MimeType mt = Utils::MimeDatabase::mimeTypeForFile(fileName);
        if (mt.isValid()) {
            bool foundProjectManager = false;
            foreach (IProjectManager *manager, projectManagers) {
                Utils::MimeType tmp = Utils::MimeDatabase::mimeTypeForName(manager->mimeType());
                if (mt == tmp) {
                    QString suberror;
                    foundProjectManager = true;
                    if (Project *pro = manager->openProject(fileName, &suberror)) {
                        // TODO: set connections or else
                        if (pro->restoreSettings()) {
                            SessionManager::addProject(pro);
                            projects += pro;
                        } else {
                            appendError(errorString,
                                        tr("Failed to open project `%1': Settings counld not be restored.")
                                        .arg(QDir::toNativeSeparators(fileName)));
                            delete pro;
                        }
                    }

                    appendError(errorString, suberror);

                    break;
                }
            }

            if (!foundProjectManager) {
                appendError(errorString,
                            tr("Failed to open project `%1': No matched plugin can open project of type `%2'.")
                            .arg(QDir::toNativeSeparators(fileName), mt.name()));
            }
        } else {
            appendError(errorString, tr("Failed to open project `%1': Unknown project type.")
                        .arg(QDir::toNativeSeparators(fileName)));
        }
    }

    return projects;
}

bool ProjectExplorerPlugin::coreAboutToClose()
{
    // TODO
    return true;
}

bool ProjectExplorerPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    TM_MARK;

    ////////////////////////////////////////////////////////////////////////////
    // initialize objects
    new SessionManager(this);

    addObject(this);
    addAutoReleasedObject(new Internal::BasicTreeManager());

    addAutoReleasedObject(new Internal::MediaSourceTimer());
    addAutoReleasedObject(new Internal::CoreListener());
    // addAutoReleasedObject(new Internal::ProjectTreeWidgetFactory());
    addAutoReleasedObject(new Internal::SegRepoTreeWidgetFactory());


    ////////////////////////////////////////////////////////////////////////////
    // register actions

    Core::Command *cmd = 0;

    // project related actions in file menu
    Core::ActionContainer *filemenu = Core::ActionManager::actionContainer(Core::Constants::M_FILE);

    dd->m_newAction = new QAction(tr("New Project..."), this);
    cmd = Core::ActionManager::registerAction(dd->m_newAction, Constants::PROJECT_NEW);
    cmd->setDefaultKeySequence(QKeySequence::New);
    filemenu->addAction(cmd, Core::Constants::G_FILE_PROJECT);
    connect(dd->m_newAction, &QAction::triggered,
            dd, &Internal::ProjectExplorerPluginPrivate::newProject);

    dd->m_openAction = new QAction(tr("Open Project..."), this);
    cmd = Core::ActionManager::registerAction(dd->m_openAction, Constants::PROJECT_OPEN);
    cmd->setDefaultKeySequence(QKeySequence::Open);
    filemenu->addAction(cmd, Core::Constants::G_FILE_PROJECT);
    connect(dd->m_openAction, &QAction::triggered,
            dd, []() { dd->openProject(); });

    Core::ActionContainer *recentmenu = Core::ActionManager::createMenu(Constants::M_ROJECT_RECENTPROJECTS);
    recentmenu->menu()->setTitle(tr("Recent Projects"));
    filemenu->addMenu(recentmenu, Core::Constants::G_FILE_PROJECT);
    recentmenu->setOnAllDisabledBehavior(Core::ActionContainer::Show);
    connect(filemenu->menu(), &QMenu::aboutToShow,
            dd, &Internal::ProjectExplorerPluginPrivate::updateRecentProjectMenu);

    // project related actions in project tree
    // TODO:

    connect(Core::ICore::instance(), &Core::ICore::saveSettingsRequested,
            dd, &Internal::ProjectExplorerPluginPrivate::savePersistentSettings);
    dd->loadPersistentSettings();

    return true;
}

void ProjectExplorerPlugin::extensionsInitialized()
{
    // TODO
    QList<IProjectManager *> projectManagers
            = ExtensionSystem::PluginManager::getObjects<IProjectManager>();
    foreach (IProjectManager *manager, projectManagers) {
        Utils::MimeDatabase::addMimeType(manager->mimeType());
    }

    Internal::MediaSourceTimer::extensionsInitialized();
}

ExtensionSystem::IPlugin::ShutdownFlag ProjectExplorerPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}



} // namespace ProjectExporer
