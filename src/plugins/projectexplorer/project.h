#ifndef PROJECT_H
#define PROJECT_H

#include <QFutureInterface>
#include <QObject>
#include <QTimer>

#include <core/icontext.h>
#include <core/id.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {
class IProjectManager;
class Node;
class FolderNode;

class MediaSource;
class Segment;
class Seg;
class Monitor;
class FastPreview;
class Sequence;

namespace Internal { class ProjectPrivate; }

class PROJECTEXPLORER_EXPORT Project : public QObject
{
    Q_OBJECT

public:
    enum ModelRoles {
          EnabledRole
    };

public:
    Project();
    virtual ~Project();

    virtual QString projectFilePath() const = 0;

    virtual QString displayName() const = 0;

    Core::Id id() const;
    Core::Context projectContext() const;
    bool isValid() const;

    // TODO:
    void addMediaSource(MediaSource *source);
    void removeMediaSource(MediaSource *source);
    QList<MediaSource *> mediaSources() const;
    MediaSource *mediaSource(Core::Id id);

    void addMonitor(Monitor *monitor);
    bool removeMonitor(Monitor *monitor);
    QList<Monitor *> monitors() const;

    void addFastPreview(FastPreview *preview);
    bool removeFastPreview(FastPreview *preview);
    QList<FastPreview *> fastPreviews() const;

    void addSequence(Sequence *seq);
    bool removeSequence(Sequence *seq);
    QList<Sequence *> sequences() const;

public:
    void saveSettings();
    bool restoreSettings();

signals:
    void displayNameChanged();
    void settingsLoaded();

    // TODO: more project related signals required
signals:
    void mediaSourceAboutToBeAdded(MediaSource *source);
    void mediaSourceAdded();

    void mediaSourceAboutToBeRemoved(MediaSource *source);
    void mediaSourceRemoved();

protected:
    void setId(Core::Id id);
    void setProjectContext(Core::Context context);

private:
    Internal::ProjectPrivate *d;
};

} // namespace ProjectExplorer

#endif // PROJECT_H
