#ifndef CLEARVISION_MEDIA_SOURCE_H
#define CLEARVISION_MEDIA_SOURCE_H

#include <projectexplorer/mediasource.h>

namespace ClearVision {
namespace Internal {

namespace PE = ProjectExplorer;

class UObject;
class MediaSourcePrivate;
class MediaSource : public PE::MediaSource
{
    Q_OBJECT

public:
    MediaSource();
    ~MediaSource();

public:
    QString className() const Q_DECL_OVERRIDE;

    Status status() const Q_DECL_OVERRIDE;
    void checkStatus() Q_DECL_OVERRIDE;

public:
    void queryRecordings(const QVariantMap &conditions) Q_DECL_OVERRIDE;
    QVariantMap lastQueryConditions() const Q_DECL_OVERRIDE;

public:
    PE::Camera *cameraOfId(Core::Id id) const Q_DECL_OVERRIDE;
    QList<PE::Segment *> segments() const Q_DECL_OVERRIDE;

public:
    void clearCameraSegments(Core::Id id) Q_DECL_OVERRIDE;
    void removeSegment(Core::Id id) Q_DECL_OVERRIDE;
    void clearSegments() Q_DECL_OVERRIDE;

public:
    QList<UObject *> childObjects(Core::Id id) const;

    bool shouldFetchMore(Core::Id id) const;
    void aboutTofetchMore(PE::MetaData *metadata = 0, bool force = false);

    // MetaData interface
public:
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;

signals:
    void objectAboutToBeAdded(UObject *obj);
    void objectAdded();

    void objectsAboutToBeAdded(Core::Id unitId, QList<UObject *> objects);
    void objectsAdded();

    void objectAboutToBeRemoved(UObject *obj);
    void objectRemoved();

    void objectsAboutToBeCleared(UObject *obj);
    void objectsCleared();

signals:
    void fetchingMore(Core::Id unitId);
    void fetchMoreFailed(Core::Id unitId, const QString &error);
    void fetchMoreSucceed(Core::Id unitId);

protected:
    void queryRouteListImpl(PE::Segment *segment, PE::IQueryRouteListCallback *callback);

private:
    MediaSourcePrivate *d;
};


} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_MEDIA_SOURCE_H
