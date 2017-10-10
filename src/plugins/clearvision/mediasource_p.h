#ifndef CLEARVISION_MEDIA_SOURCE_P_H
#define CLEARVISION_MEDIA_SOURCE_P_H

#include <QMutex>

#include <mediasource/constants.h>
#include <projectexplorer/iqueryroutelistcallback.h>
#include <projectexplorer/mediasource.h>
#include <utils/communicator/udocacommunicator.h>
#include <utils/communicator/udocacontext.h>

#include "devices.h"
#include "devices_p.h"
#include "segment.h"

namespace ClearVision {
namespace Internal {

namespace PE = ProjectExplorer;
namespace MS = MediaSource;

class MediaSource;
class MediaSourcePrivate : public Utils::CommunicatorCallback
{
    Q_OBJECT

public:
    MediaSourcePrivate(MediaSource *mediasource);
    ~MediaSourcePrivate();

public:
    void aboutToFetchMore(Unit *unit);
    void queryRecordings(const QVariantMap &conditions);
    void queryRouteList(Segment *segment, PE::IQueryRouteListCallback *callback);

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
    void estatblished();

private:
    void onGetProfileResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp);
    void onGetOrganizationalObjectsResponseReceived(Core::Id unitId, const UDOCA::Request &req, const UDOCA::Response &resp);
    void onQueryRecordingsByCameraResponseReceived(const QVariantMap &conditions, const UDOCA::Request &req, const UDOCA::Response &resp);
    void onGetPlaybackServerRouteListResponseReceived(Segment *segment, PE::IQueryRouteListCallback *callback, const UDOCA::Request &req, const UDOCA::Response &resp);

private:
    Domain *domainOfObj(Core::Id id);
    Domain *domainOfObj(UObject *obj);
    void clearObjects(Unit *unit = 0);
    void afterEstablished();

public:
    Utils::Communicator m_communicator;

public:
    MediaSource *m_mediasource;
    PE::MediaSource::Status m_mediaSourceStatus;

public:
    QString m_username;
    QString m_password;

    QString m_serverAddr;
    quint16 m_serverPort;

public:
    mutable QMutex m_objectLock;
    QHash<Core::Id, UObject *> m_objects;                // delete here
    QHash<Core::Id, QList<UObject *> > m_childObjects;   // treat as ready if contains id

public:
    mutable QMutex m_queryLock;
    QHash<Core::Id, PE::Segment *> m_segments;
    QVariantMap m_lastQueryConditions;
};

} // namespace Internal
} // namespace ClearVision


#endif // CLEARVISION_MEDIA_SOURCE_P_H
