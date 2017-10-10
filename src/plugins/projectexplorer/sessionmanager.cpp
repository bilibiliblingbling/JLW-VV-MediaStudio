#include "project.h"
#include "sessionmanager.h"

/*!
    \class SessionManager 管理所有工程
    负责维护所有工程相关内容, 添加, 删除

 */

namespace ProjectExplorer {
namespace Internal {
class SessionManagerPrivate
{
public:
    Project *m_startupProject;          // 当前工作 Project
    QList<Project *> m_projects;
};
} // namespace Internal


static SessionManager *s_instance = 0;

SessionManager::SessionManager(QObject *parent)
    : QObject(parent)
    , d(new Internal::SessionManagerPrivate())
{
    s_instance = this;

    d->m_startupProject = 0;
}

SessionManager::~SessionManager()
{
    qDeleteAll(d->m_projects);
    delete d;
}

SessionManager *SessionManager::instance()
{
    return s_instance;
}

void SessionManager::addProject(Project *project)
{
    addProjects(QList<Project*>() << project);
}

void SessionManager::addProjects(const QList<Project *> &projects)
{
    QList<Project*> clearedList;
    foreach (Project *pro, projects) {
        if (!instance()->d->m_projects.contains(pro)) {
            clearedList.append(pro);
            instance()->d->m_projects.append(pro);
        }
    }

    foreach (Project *pro, clearedList) {
        emit instance()->projectAdded(pro);
    }
}

void SessionManager::removeProject(Project *project)
{
    Q_ASSERT(project);

    removeProjects(QList<Project *>() << project);
}

void SessionManager::removeProjects(QList<Project *> projects)
{
    // TODO
}

void SessionManager::setStartupProject(Project *project)
{
    if (project) {
        Q_ASSERT(instance()->d->m_projects.contains(project));
    }

    if (instance()->d->m_startupProject == project)
        return;

    instance()->d->m_startupProject = project;

    emit instance()->startupProjectChanged(project);
}

/*!
 * \brief SessionManager::startupProject
 * \return 当前处于激活态的 Project
 */
Project *SessionManager::startupProject()
{
    return instance()->d->m_startupProject;
}

const QList<Project *> &SessionManager::projects()
{
    return instance()->d->m_projects;
}

bool SessionManager::save()
{
    // TODO
    return true;
}

void SessionManager::closeAllProjects()
{
    // TODO
}

} // namespace ProjectExplorer
