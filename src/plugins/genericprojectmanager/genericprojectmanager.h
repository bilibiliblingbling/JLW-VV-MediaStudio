#ifndef GENERIC_PROJECT_MANAGER_H
#define GENERIC_PROJECT_MANAGER_H

#include <projectexplorer/iprojectmanager.h>

namespace GenericProjectManager {
namespace Internal {

class GenericProjectManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    GenericProjectManager();

    // IProjectManager interface
public:
    QString mimeType() Q_DECL_OVERRIDE;
    ProjectExplorer::Project *openProject(const QString &filename, QString *errorString) Q_DECL_OVERRIDE;
};

} // namespace Internal
} // namespace GenericProjectManager

#endif // GENERIC_PROJECT_MANAGER_H
