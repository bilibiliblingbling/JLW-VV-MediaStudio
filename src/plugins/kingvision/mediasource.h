#ifndef KINGVISION_MEDIA_SOURCE_H
#define KINGVISION_MEDIA_SOURCE_H

#include <projectexplorer/mediasource.h>

#include "devices.h"

namespace KingVision {
namespace Internal {

namespace PE = ProjectExplorer;

class MediaSourcePrivate;
class MediaSource : public PE::MediaSource
{
    Q_OBJECT

public:
    MediaSource();
    ~MediaSource();

public:
    QList<Codec *> codecs() const;
    QList<Camera *> cameras() const;

    // MediaSource interface
public:
    QString className() const Q_DECL_OVERRIDE;

    Status status() const Q_DECL_OVERRIDE;
    void checkStatus() Q_DECL_OVERRIDE;

public:
    void startRecording(Core::Id cameraId);
    void stopRecording(Core::Id cameraId);
    void startRecordingAll();
    void stopRecordingAll();

public:
    void queryRecordings(const QVariantMap &conditions) Q_DECL_OVERRIDE;
    QVariantMap lastQueryConditions() const Q_DECL_OVERRIDE;

public:
    QList<PE::Segment *> segments() const Q_DECL_OVERRIDE;
    Camera *cameraOfId(Core::Id id) const Q_DECL_OVERRIDE;

public:
    void clearCameraSegments(Core::Id id) Q_DECL_OVERRIDE;
    void removeSegment(Core::Id id) Q_DECL_OVERRIDE;
    void clearSegments() Q_DECL_OVERRIDE;

    // MetaData interface
public:
    bool fromMap(const QVariantMap &map);
    QVariantMap toMap() const;

signals:
    void codecAboutToBeAdded(Codec *codec);
    void codecAdded();

    void codecAboutToBeRemoved(Codec *codec);
    void codecRemoved();

    void cameraAboutToBeAdded(Camera *camera);
    void cameraAdded();

    void cameraAboutToBeRemoved(Camera *camera);
    void cameraRemoved();

    void codecsAboutToBeAdded(const QList<Codec *> &codecs);
    void codecsAdded();

    void codecsAboutToBeRemoved();
    void codecsRemoved();

    void codecsAboutToBeCleared();
    void codecsCleared();

    void camerasAboutToBeAdded();
    void camerasAdded();

    void camerasAboutToBeRemoved();
    void camerasRemoved();
    void camerasAboutToBeCleared();
    void camerasCleared();

private:
    MediaSourcePrivate *d;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_MEDIA_SOURCE_H
