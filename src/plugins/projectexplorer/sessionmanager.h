#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <QObject>

#include "projectexplorer_global.h"

namespace ProjectExplorer {
class Project;

namespace Internal { class SessionManagerPrivate; }

class PROJECTEXPLORER_EXPORT SessionManager : public QObject
{
    Q_OBJECT

public:
    explicit SessionManager(QObject *parent = 0);
    ~SessionManager();

    static SessionManager *instance();

public:
    static void addProject(Project *project);
    static void addProjects(const QList<Project*> &projects);
    static void removeProject(Project *project);
    static void removeProjects(QList<Project *> projects);

    static void setStartupProject(Project *project);
    static Project *startupProject();
    static const QList<Project *> &projects();

public:
    static bool save();
    static void closeAllProjects();

signals:
    void startupProjectChanged(Project *currentStartupProject);

    void projectAdded(ProjectExplorer::Project *project);
    void singleProjectAdded(ProjectExplorer::Project *project);

    void aboutToRemoveProject(ProjectExplorer::Project *project);
    void projectRemoved(ProjectExplorer::Project *project);

    void projectDisplayNameChanged(ProjectExplorer::Project *project);

private slots:

private:
    Internal::SessionManagerPrivate *d;
};

} // namespace ProjectExplorer

#endif // SESSION_MANAGER_H
