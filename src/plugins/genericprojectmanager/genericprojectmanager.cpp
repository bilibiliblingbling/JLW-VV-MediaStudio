#include <QFileInfo>

#include "constants.h"
#include "genericproject.h"
#include "genericprojectmanager.h"

namespace GenericProjectManager {
namespace Internal {

GenericProjectManager::GenericProjectManager()
{
    setObjectName(QStringLiteral("GenericProjectManager.GenericProjectManager"));
}

QString GenericProjectManager::mimeType()
{
    return QLatin1String(Constants::GENERICPROJECT_MIMETYPE);
}

ProjectExplorer::Project *GenericProjectManager::openProject(const QString &filename, QString *errorString)
{
    if (!QFileInfo(filename).isFile()) {
        if (errorString) {
            *errorString = tr("Fail to open project: `%1' is not a file").arg(filename);
        }

        return 0;
    }

    GenericProject *pro = new GenericProject(this, filename);
    if (pro && !pro->parseSettings(errorString)) {
        delete pro;
        pro = 0;
    }

    return pro;
}

} // namespace Internal
} // namespace GenericProjectManager
