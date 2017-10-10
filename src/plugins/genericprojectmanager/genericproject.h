#ifndef GENERIC_PROJECT_H
#define GENERIC_PROJECT_H

#include <projectexplorer/project.h>

namespace GenericProjectManager {
namespace Internal {
class GenericProjectManager;

class GenericProject : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    GenericProject(GenericProjectManager *manager, const QString &filename);
    ~GenericProject();

public:
    bool parseSettings(QString *errorString = 0);

    // Project interface
public:
    QString projectFilePath() const;
    QString displayName() const;

private:
    bool parseProjectInfo(QString *errorString);
    bool parseMediaSourceInfo(QString *errorString);
    bool parseMediaDestinationInfo(QString *errorString);
    bool parseMediaFileInfo(QString *errorString);
    bool parseFastPreviewInfo(QString *errorString);
    bool parsePlayMonitorInfo(QString *errorString);
    bool parseSequenceInfo(QString *errorString);

private:
    GenericProjectManager *m_manager;

private:
    QString m_projectName;

private:
    QString m_projectFileName;
    QString m_mediaSourceFileName;
    QString m_fastPreviewFileName;
    QString m_mediaFileFileName;
    QString m_mediaDestinationFileName;
    QString m_sequenceFileName;
    QString m_playMonitorFileName;
};

} // namespace Internal
} // namespace GenericProjectManager


#endif // GENERIC_PROJECT_H
