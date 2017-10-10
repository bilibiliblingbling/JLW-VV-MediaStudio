#ifndef IPROJECT_MANAGER_H
#define IPROJECT_MANAGER_H

#include <QObject>

#include "projectexplorer_global.h"

namespace ProjectExplorer {
class Project;

class PROJECTEXPLORER_EXPORT IProjectManager : public QObject
{
    Q_OBJECT

public:
    virtual ~IProjectManager();

public:
    virtual QString mimeType() = 0;
    virtual Project *openProject(const QString &filename, QString *errorString = 0) = 0;
};

} // namespace ProjectExplorer

#endif // IPROJECT_MANAGER_H
