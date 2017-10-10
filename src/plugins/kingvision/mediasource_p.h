#ifndef KINGVISION_MEDIA_SOURCE_P_H
#define KINGVISION_MEDIA_SOURCE_P_H

#include <QMutex>

#include <core/id.h>
#include <projectexplorer/mediasource.h>
#include <utils/communicator/udocacommunicator.h>
#include <utils/communicator/udocacontext.h>

#include "devices.h"
#include "segment.h"

namespace KingVision {
namespace Internal {

namespace PE = ProjectExplorer;

class MediaSource;
class MediaSourcePrivate : public Utils::CommunicatorCallback
{
    Q_OBJECT

public:
    MediaSourcePrivate(MediaSource *source);
    ~MediaSourcePrivate();

public:
    void startRecording (Core::Id cameraId);
    void stopRecording(Core::Id cameraId);
    void startRecordingAll();
    void stopRecordingAll();

    void queryRecordings(const QVariantMap &conditions);

    // CommunicatorCallback interface
public:
    void onConnecting(const UDOCA::Endpoint &peer);
    void onConnected(const UDOCA::Endpoint &local, const UDOCA::Endpoint &peer);
    void onConnectFailed(const UDOCA::Endpoint &peer, const UDOCA::Exception &e);

    void onEstablished();
    void onEstablishedFailed(const UDOCA::Endpoint &peer, const UDOCA::Exception &e);

    void onResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp, UDOCA::IContext *ctxt);
    void onRequestReceived(const UDOCA::Request &req);

    void onTerminated(const UDOCA::TerminateReason &reason);

signals:
    void established();
    void serviceConfigGot();

private:
    void onGetServiceConfigResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp);
    void onGetDeviceListResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp);
    void onGetCameraListResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp);
    void onQueryRecordingsByCameraResponseReceived(const QVariantMap &conditions, const UDOCA::Request &req, const UDOCA::Response &resp);

    void onStartRecordResponseReceived(Core::Id cameraId, const UDOCA::Request &req, const UDOCA::Response &resp);
    void onStopRecordResponseReceived(Core::Id cameraId, const UDOCA::Request &req, const UDOCA::Response &resp);
    void onStartRecordAllResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp);
    void onStopRecordAllResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp);

private:
    void afterEstablished();
    void afterGetServiceConfig();

public:
    Utils::Communicator m_communicator;

public:
    MediaSource *m_mediasource;
    PE::MediaSource::Status m_mediaSourceStatus;

public:
    QString m_storageProxyAddr;
    quint16 m_storageProxyPort;

    quint16 m_liveStreamPort;
    quint16 m_recordStreamPort;

public:
    mutable QMutex m_deviceLock;
    QHash<Core::Id, Codec *> m_codecs;
    QHash<Core::Id, Camera *> m_cameras;

public:
    mutable QMutex m_queryLock;
    QHash<Core::Id, PE::Segment *> m_segments;
    QVariantMap m_lastQueryConditions;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_MEDIA_SOURCE_P_H
