#ifndef MEDIA_SOURCE_H
#define MEDIA_SOURCE_H

#include "camera.h"
#include "metadata.h"
#include "segment.h"

namespace ProjectExplorer {

class Project;
class IQueryRouteListCallback;
class PROJECTEXPLORER_EXPORT MediaSource : public MetaData
{
    Q_OBJECT

public:
    enum Status {
          StatusDisabled
        , StatusLoggingIn
        , StatusOnline
        , StatusOffline
    };

public:
    MediaSource(Core::Id classId);
    MediaSource(Project *pro, Core::Id id, Core::Id clsId);
    virtual ~MediaSource();

public:
    Core::Id classId() const;
    virtual QString className() const;

public:
    bool enabled() const;
    void setEnabled(bool enabled);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

public:
    virtual Status status() const = 0;
    virtual void checkStatus() = 0;

public:
    virtual void queryRecordings(const QVariantMap &conditions) = 0;
    virtual QVariantMap lastQueryConditions() const = 0;

public:
    virtual Camera *cameraOfId(Core::Id id) const = 0;
    virtual QList<Segment *> segments() const = 0;

public:
    virtual void clearCameraSegments(Core::Id id) = 0;
    virtual void removeSegment(Core::Id id) = 0;
    virtual void clearSegments() = 0;

public:
    static void queryRouteList(Segment *segment, IQueryRouteListCallback *callback);

public:
    static Core::Id standardClassId(Core::Id id);
    static Core::Id standardClassIdPrefix();

signals:
    void segmentAboutToBeAdded(Segment *segment);
    void segmentAdded();

    void segmentAboutToBeRemoved(Segment *segment);
    void segmentRemoved();

    void cameraSegmentsAboutToBeCleared(Camera *camera);
    void cameraSegmentsCleared();

    void segmentsAboutToBeRemoved(const QList<Segment *> &segments);
    void segmentsRemoved();

    void segmentsAboutToBeCleared();
    void segmentsCleared();
    // TODO: more segment related signals may be required

    // TODO:

    // MetaData interface
public:
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;

protected:
    virtual void queryRouteListImpl(Segment *segment, IQueryRouteListCallback *callback);

private:
    Core::Id m_classId;
    bool m_enabled;
    QString m_name;
    QString m_description;
};

} // namespace ProjectExplorer

#endif // MEDIA_SOURCE_H
