#ifndef PROJECT_EXPLORER_PLUGIN_H
#define PROJECT_EXPLORER_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {
class Project;
class ProjectManager;

class PROJECTEXPLORER_EXPORT ProjectExplorerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "ProjectExplorer.json")

public:
    ProjectExplorerPlugin();
    ~ProjectExplorerPlugin();

    static ProjectExplorerPlugin *instance();

public:
    static Project *openProject(const QString &fileName, QString *errorString = 0);
    static QList<Project *> openProjects(const QStringList &fileNames, QString *errorString);

public:
    static bool coreAboutToClose();

signals:
    void recentProjectsChanged();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
};


} // namespace ProjectExplorer

#endif // PROJECT_EXPLORER_PLUGIN_H
