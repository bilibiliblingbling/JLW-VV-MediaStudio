#include <QCoreApplication>

// ugly, required by StreamSDK
#include <swift/std/cstdint.hpp>
#include <StreamSDK/Route.h>
#include <StreamSDK/Stream.h>

#include <share/Constants.h>
#include <share/UserProfile.h>

#include <utils/tmplog.h>

#include "constants.h"
#include "devices.h"
#include "mediasource.h"
#include "mediasource_p.h"

#define DUMP_OBJECT_TO_XML
#undef DUMP_OBJECT_TO_XML

namespace ClearVision {
namespace Internal {

MediaSource::MediaSource()
    : PE::MediaSource(Constants::MEDIASOURCE_CLASSID)
    , d(new MediaSourcePrivate(this))
{
}

MediaSource::~MediaSource()
{
    delete d;
}

QString MediaSource::className() const
{
    return QCoreApplication::translate("ClearVision", Constants::TR_MEDIASOURCE_CLASSNAME);
}

MediaSource::Status MediaSource::status() const
{
    return d->m_mediaSourceStatus;
}

void MediaSource::checkStatus()
{
    if (d->m_mediaSourceStatus == StatusOffline) {
        d->m_mediaSourceStatus = StatusLoggingIn;
        UDOCA::Endpoint ep("http", d->m_serverAddr.toStdString(), d->m_serverPort);
        d->m_communicator.establish(ep, d->m_username, d->m_password);
        return;
    }

    // TODO:
}

void MediaSource::queryRecordings(const QVariantMap &conditions)
{
    d->queryRecordings(conditions);
}

QVariantMap MediaSource::lastQueryConditions() const
{
    return d->m_lastQueryConditions;
}

PE::Camera *MediaSource::cameraOfId(Core::Id id) const
{
    UObject *obj = d->m_objects.value(id);
    if (obj) {
        return obj->asCamera();
    }
    return 0;
}

QList<PE::Segment *> MediaSource::segments() const
{
    return d->m_segments.values();
}

void MediaSource::clearCameraSegments(Core::Id id)
{
    PE::Camera *camera = cameraOfId(id);
    if (camera) {
        emit cameraSegmentsAboutToBeCleared(camera);

        QList <Core::Id> segments;
        foreach (auto seg, d->m_segments) {
            if (seg->cameraId() == id) {
                segments.append(seg->infoId());
            }
        }

        foreach (auto seg, segments) {
            d->m_segments.remove(seg);
        }

        emit cameraSegmentsCleared();
    }
}

void MediaSource::removeSegment(Core::Id id)
{
    foreach (auto seg, d->m_segments) {
        if(seg->id() == id) {
            emit segmentAboutToBeRemoved(seg);
            d->m_segments.remove(seg->infoId());
            emit segmentRemoved();

            break;
        }
    }
}

void MediaSource::clearSegments()
{
    emit segmentsAboutToBeCleared();
    d->m_segments.clear();
    emit segmentsCleared();
}

QList<UObject *> MediaSource::childObjects(Core::Id id) const
{
    return d->m_childObjects.value(id);
}

bool MediaSource::shouldFetchMore(Core::Id id) const
{
    return !d->m_childObjects.contains(id);
}

void MediaSource::aboutTofetchMore(MetaData *metadata, bool force)
{
    Core::Id unitId = metadata? metadata->id(): Core::Id::fromString(Constants::CLEARVISION_OBJECT_ROOTID);

    if (!d->m_childObjects.contains(unitId) || force) {
        d->m_childObjects.insert(unitId, QList<UObject *>());
        emit fetchingMore(unitId);
        d->aboutToFetchMore(qobject_cast<Unit *>(metadata));
    }
}

bool MediaSource::fromMap(const QVariantMap &map)
{
    if (!PE::MediaSource::fromMap(map))
        return false;

    QVariantMap arguments = map.value(QLatin1String(Constants::MEDIASOURCE_ARGUMENTS)).toMap();
    d->m_username = arguments.value(QLatin1String(Constants::MEDIASOURCE_USERNAME)).toString();
    d->m_password = arguments.value(QLatin1String(Constants::MEDIASOURCE_PASSWORD)).toString();
    d->m_serverAddr = arguments.value(QLatin1String(Constants::MEDIASOURCE_SERVERADDR)).toString();
    d->m_serverPort = arguments.value(QLatin1String(Constants::MEDIASOURCE_SERVERPORT), 0).toUInt();
    if (!d->m_serverPort)
        d->m_serverPort = Constants::MEDIASOURCE_SERVERPORT_DEFAULT;

    return true;
}

QVariantMap MediaSource::toMap() const
{
    QVariantMap map(PE::MediaSource::toMap());

    QVariantMap arguments;
    arguments.insert(QLatin1String(Constants::MEDIASOURCE_USERNAME), d->m_username);
    arguments.insert(QLatin1String(Constants::MEDIASOURCE_PASSWORD), d->m_password);
    arguments.insert(QLatin1String(Constants::MEDIASOURCE_SERVERADDR), d->m_serverAddr);
    arguments.insert(QLatin1String(Constants::MEDIASOURCE_SERVERPORT), d->m_serverPort);

    map.insert(QLatin1String(Constants::MEDIASOURCE_ARGUMENTS), arguments);

    return map;
}

void MediaSource::queryRouteListImpl(PE::Segment *segment, PE::IQueryRouteListCallback *callback)
{
    if (Segment *seg = qobject_cast<Segment *>(segment)) {
        d->queryRouteList(seg, callback);
    } else {
        PE::MediaSource::queryRouteListImpl(segment, callback);
    }
}


////////////////////////////////////////////////////////////////////////////////


MediaSourcePrivate::MediaSourcePrivate(MediaSource *mediasource)
    : CommunicatorCallback(mediasource)
    , m_communicator(Constants::MEDIASOURCE_CLEARVISION_ROLE, *this)
    , m_mediasource(mediasource)
    , m_mediaSourceStatus(PE::MediaSource::StatusOffline)
    , m_objectLock(QMutex::Recursive)
    , m_queryLock(QMutex::Recursive)
{
    UDOCA::String::setLocalCharset(UDOCA::String::CHARSET_UTF8);

    connect(this, &MediaSourcePrivate::estatblished, this, [this]() { afterEstablished(); });
}

MediaSourcePrivate::~MediaSourcePrivate()
{
}

void MediaSourcePrivate::aboutToFetchMore(Unit *unit)
{
    // TODO: clear ;
    clearObjects(unit);

    UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
    req.setClassName(Constants::CLEARVISION_CENTRALSERVER);
    req.setMethodName("GetOrganizationalObjects");
    req.setParameter("ObjectClasses", CV_OBJECT_CLASS_OU | CV_OBJECT_CLASS_DOMAIN | CV_OBJECT_CLASS_CAMERA | CV_OBJECT_CLASS_NETWORK_CAMERA);

    UDOCA::List properties = UDOCA::List::create();
    properties.add(Constants::CLEARVISION_OBJECT_ID);
    properties.add(Constants::CLEARVISION_OBJECT_NAME);
    properties.add(Constants::CLEARVISION_OBJECT_CLASS);
    properties.add(Constants::CLEARVISION_OBJECT_ORGPATH);
    req.setParameter("PropertyNameList", properties);

    if (unit) {
        req.setParameter(Constants::CLEARVISION_OBJECT_ORGUNITID, unit->unitId());
    }

    if (Domain *domain = domainOfObj(unit)) {
        req.setDestination(UDOCA::ObjectIdentity("", Constants::CLEARVISION_CENTRALSERVER, domain->id().toString().toStdString()));
#ifdef DUMP_OBJECT_TO_XML
        TM_DBG << "------------ foreign domain request -----------------------";
        TM_DBG << req.toXML().getData();
#endif
    }

    Core::Id unitId = unit? unit->id(): Constants::CLEARVISION_OBJECT_ROOTID;
    Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
    connect(ctxt, &Utils::UDOCAContext::responseReceived,
            this, [this, unitId](const UDOCA::Request &req, const UDOCA::Response &resp) { onGetOrganizationalObjectsResponseReceived(unitId, req, resp); });

    m_communicator.sendRequest(req, ctxt);
}

void MediaSourcePrivate::queryRecordings(const QVariantMap &conditions)
{
    // append or erase
    bool append = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_APPENDRESULTS)).toBool();
    if (!append) {
        emit m_mediasource->segmentsAboutToBeCleared();
        qDeleteAll(m_segments);
        m_segments.clear();
        emit m_mediasource->segmentsCleared();
    }

    QStringList cameraList = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_CAMERALIST)).toStringList();
    qint32 recordType = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_RECORDTYPE), 7).toInt();
    quint64 bgnTime = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_BGNTIME)).toULongLong();
    quint64 endTime = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_ENDTIME)).toULongLong();
    UDOCA::InvokeMethodRequest req_ = UDOCA::InvokeMethodRequest::create();
    req_.setClassName(Constants::CLEARVISION_CENTRALSERVER);
    req_.setMethodName("QueryRecordingsByCamera");
    req_.setParameter("RecordType", recordType);
    req_.setParameter("BeginTime", UDOCA::DateTime(bgnTime));
    req_.setParameter("EndTime", UDOCA::DateTime(endTime));

    // may cross domains
    QMap<UDOCA::String, UDOCA::List> domainCameras; // domain -> cameras
    if (cameraList.empty()) {
        // query local domain recordings if no camera specified
        domainCameras.insert(UDOCA::String(), UDOCA::List::create());
    }

    foreach (const QString &camera, cameraList) {
        Domain *domain = domainOfObj(Core::Id::fromString(camera));
        UDOCA::String domainId = domain? domain->objId(): UDOCA::String();

        if (!domainCameras.contains(domainId))
            domainCameras.insert(domainId, UDOCA::List::create());

        UDOCA::List camlst = domainCameras[domainId];
        camlst.add(UDOCA::String(camera.toStdString()));
    }

    QMap<UDOCA::String, UDOCA::List>::const_iterator it = domainCameras.begin();
    for (; it != domainCameras.end(); ++it) {
        Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
        connect(ctxt, &Utils::UDOCAContext::responseReceived,
                this, [this, conditions](const UDOCA::Request &req, const UDOCA::Response &resp) { onQueryRecordingsByCameraResponseReceived(conditions, req, resp); });

        UDOCA::InvokeMethodRequest req(req_.clone());
        req.setParameter("CameraList", it.value());
        if (!it.key().isEmpty()) {
            req.setDestination(UDOCA::ObjectIdentity("", Constants::CLEARVISION_CENTRALSERVER, it.key()));
        }

        m_communicator.sendRequest(req, ctxt);
    }
}

void MediaSourcePrivate::queryRouteList(Segment *segment, PE::IQueryRouteListCallback *callback)
{
    UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
    req.setClassName(Constants::CLEARVISION_CENTRALSERVER);
    req.setMethodName("GetPlaybackServerRouteList");

    req.setParameter("ID", segment->serverId());
    Domain *domain = domainOfObj(segment->cameraId());
    if (domain) {
        req.setDestination(UDOCA::ObjectIdentity("", Constants::CLEARVISION_CENTRALSERVER, domain->objId()));
    }

    Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
    connect(ctxt, &Utils::UDOCAContext::responseReceived,
            this, [this, segment, callback](const UDOCA::Request &req, const UDOCA::Response &resp) { onGetPlaybackServerRouteListResponseReceived(segment, callback, req, resp); });

    m_communicator.sendRequest(req, ctxt);
}

void MediaSourcePrivate::onConnecting(const UDOCA::Endpoint &peer)
{
    // TODO: update node status, blinkable
}

void MediaSourcePrivate::onConnected(const UDOCA::Endpoint &local, const UDOCA::Endpoint &peer)
{
    Q_UNUSED(local);
    Q_UNUSED(peer);
}

void MediaSourcePrivate::onConnectFailed(const UDOCA::Endpoint &peer, const UDOCA::Exception &e)
{
    TM_DBG("failed to establish connect with [%s], error info, %s",
           peer.toUrl().getData(), e.getMessage().getData());
    // TODO: post error info
    m_mediaSourceStatus = MediaSource::StatusOffline;
}

void MediaSourcePrivate::onEstablished()
{
    TM_MARK;
    emit estatblished();
}

void MediaSourcePrivate::onEstablishedFailed(const UDOCA::Endpoint &peer, const UDOCA::Exception &e)
{
    TM_DBG("failed to establish connect with [%s], error info, %s",
           peer.toUrl().getData(), e.getMessage().getData());
    // TODO: post error info

    m_mediaSourceStatus = MediaSource::StatusOffline;
}

void MediaSourcePrivate::onResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp, UDOCA::IContext *ctxt)
{
    auto udocactxt = dynamic_cast<Utils::UDOCAContext *>(ctxt);
    Q_ASSERT(udocactxt);
    if (udocactxt) {
        emit udocactxt->responseReceived(req, resp);
        if (!resp.hasMoreResponse()) {
            udocactxt->deleteLater();
        } else {
            TM_DBG << "has more response";
        }
    }
}

void MediaSourcePrivate::onRequestReceived(const UDOCA::Request &req)
{
#if 0
    TM_MARK;
#ifdef DUMP_OBJECT_TO_XML
    TM_DBG << "----------------------- Request Received ----------------------";
    TM_DBG << req.toXML().getData();
#endif
#endif
}

void MediaSourcePrivate::onTerminated(const UDOCA::TerminateReason &reason)
{
    TM_MARK;
    TM_DBG << "Terminated, reason:" << reason.getMessage().getData();
}

void MediaSourcePrivate::onGetProfileResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);
    if (!verifyResponse(resp)) {
        m_mediaSourceStatus = MediaSource::StatusOffline;
        m_communicator.terminate();
        return;
    }

    UserProfile::Instance().Initialize(resp);

    m_mediaSourceStatus = MediaSource::StatusOnline;
}

void MediaSourcePrivate::onGetOrganizationalObjectsResponseReceived(Core::Id unitId, const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);
    TM_MARK << "get children of unit:" << unitId.toString().toStdString();
    QString error;
    if (!verifyResponse(resp, &error)) {
        if (m_childObjects.contains(unitId)) {
            m_childObjects.remove(unitId);
        }
        emit m_mediasource->fetchMoreFailed(unitId, error);
        return;
    }

    emit m_mediasource->fetchMoreSucceed(unitId);

    QList<UObject *> chilren = m_childObjects.value(unitId);

    TM_DBG << "---------------- previous chilren of unitId:" << unitId.toString().toStdString()
           << "size:" << chilren.size();
    foreach (UObject *obj, chilren) {
        TM_DBG("%s", obj->objId().getData());
    }

    UDOCA::InvokeMethodResponse imresp(resp);
    UDOCA::List list = imresp.getReturn().toList();
    TM_DBG << "objects size:" << list.size();
    UDOCA::Iterator it = list.iterator();
    while (it.hasNext()) {
        UDOCA::Object obj = it.next();
#ifdef DUMP_OBJECT_TO_XML
        TM_DBG << "--------------------- Objects -----------------------------";
        TM_DBG << UDOCA::ObjectToXML(obj).getData();
#endif
        // TODO: optimization required, use add chilren etc
        if (obj.isValid()) {
            // fix organizational unit id for chilren of domains;
            obj.setProperty(Constants::CLEARVISION_OBJECT_ORGUNITID, unitId.toString().toStdString());
            UObject *o = UObject::create(obj);
            if (o) {
                TM_DBG("new object, id: %s, name: %s", o->objId().getData(), o->objName().toStdString().c_str());
                Domain *domain = domainOfObj(o);
                if (UserProfile::Instance()
                        .HasPermission(domain? domain->objId(): UDOCA::String(), o->objPath(), o->objClass(),
                                       CV_CAMERA_PERMISSION_PLAYBACK)) {
                    // check permission
                    m_objects.insert(o->objIId(), o);
                    chilren.append(o);
                } else {
                    delete o;
                }
            }
        }
    }

    emit m_mediasource->objectsAboutToBeAdded(unitId, chilren);
    m_childObjects[unitId] = chilren;
    emit m_mediasource->objectsAdded();
}

void MediaSourcePrivate::onQueryRecordingsByCameraResponseReceived(const QVariantMap &conditions, const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);

    if (!verifyResponse(resp, 0)) {
        m_lastQueryConditions = conditions;
        return;
    }
    // TODO: handle errors

    QMutexLocker locker(&m_queryLock);

    // playback server id
    UDOCA::ObjectIdentity objIdentity = resp.getSource();
    UDOCA::String serverId = objIdentity.getID();

    UDOCA::InvokeMethodResponse imresp(resp);
    UDOCA::List segments = imresp.getReturn().toList();
    UDOCA::Iterator it = segments.iterator();
    while (it.hasNext()) {
        UDOCA::Object obj = it.next();
        if (!obj.isValid())
            continue;

#ifdef DUMP_OBJECT_TO_XML
        TM_DBG << "--------------------- Recordings -------------------------------";
        TM_DBG << UDOCA::ObjectToXML(obj).getData();
#endif

        // needed when query segment route list
        obj.setProperty(Constants::CLEARVISION_SERVERID, serverId);

        Segment *segment = Segment::create(m_mediasource->id(), obj);
        if (segment && segment->isValid()) {
            if (m_segments.contains(segment->infoId()))
                continue;

            emit m_mediasource->segmentAboutToBeAdded(segment);
            m_segments.insert(segment->infoId(), segment);
            emit m_mediasource->segmentAdded();
        }
    }

    m_lastQueryConditions = conditions;
}

void MediaSourcePrivate::onGetPlaybackServerRouteListResponseReceived(Segment *segment, PE::IQueryRouteListCallback *callback, const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);
    QString errString;
    if (!verifyResponse(resp, &errString)) {
        callback->onQueryRouteListFailed(segment);
        return;
    }

    try {
        StreamSDK::RouteList rl;
        UDOCA::InvokeMethodResponse imresp(resp);
        UDOCA::List routelist = imresp.getReturn().toList();
        UDOCA::Iterator it = routelist.iterator();
        while (it.hasNext()) {
            UDOCA::Object obj = it.next();
            if (!obj.isValid())
                continue;

#ifdef DUMP_OBJECT_TO_XML
            TM_DBG << "-------- PlaybackServer Route List --------------------";
            TM_DBG << UDOCA::ObjectToXML(obj).getData();
#endif
            UDOCA::String endpoint = obj.getProperty("Endpoint").toString();

            StreamSDK::StreamUrl url(StreamSDK::Endpoint(endpoint.getData()),
                                     segment->cameraId().toString().toStdString().c_str(),
                                     segment->bgnTime(), segment->endTime());
            if (!url.IsValid())
                continue;

            StreamSDK::Token token;
            {
                UDOCA::Object token_ = obj.getProperty("Token");

                UDOCA::UInt64 expiredtime(token_.getProperty("ExpiredTime"));
                UDOCA::Binary nonce(token_.getProperty("Nonce"));
                UDOCA::Binary digest(token_.getProperty("Digest"));
                token = StreamSDK::Token(expiredtime, nonce.data(), nonce.length(), digest.data(), digest.length());

                if (token_.hasProperty("Parameters")) {
                    UDOCA::NamedObjectList params(token_.getProperty("Parameters").getPropertyList());
                    UDOCA::NamedObjectIterator pit = params.iterator();
                    while (pit.hasNext()) {
                        UDOCA::NamedObject no = pit.next();
                        token.SetParameter(no.getName().getData(), no.getValue().toString().getData());
                    }
                }
                token.SetParameter("ParameterValueEncoding", "UTF8");
            }

            rl.PushEntry(StreamSDK::RouteEntry(url, token));
        }

        callback->onQueryRouteListSuccess(segment, rl);
        return;
    } catch (const UDOCA::Exception &e) {
        TM_DBG << "failed to get playback server route list:" << e.getMessage().getData();
    } catch (...) {
        TM_DBG << "failed to get playback server route list";
    }

    callback->onQueryRouteListFailed(segment);
}

Domain *MediaSourcePrivate::domainOfObj(Core::Id id)
{
    return domainOfObj(m_objects.value(id));
}

Domain *MediaSourcePrivate::domainOfObj(UObject *obj)
{
    if (!obj)
        return 0;

    if (Domain *domain = obj->asDomain()) {
        return domain;
    }

    if ((obj = m_objects.value(obj->objUnitIId()))) {
        return domainOfObj(obj);
    }

    return 0;
}

void MediaSourcePrivate::clearObjects(Unit *unit)
{
    // clear all objects
    QMutexLocker locker(&m_objectLock);

    emit m_mediasource->objectsAboutToBeCleared(unit);
    if (unit) {

    } else {
        m_childObjects.clear();
        qDeleteAll(m_objects);
    }
    emit m_mediasource->objectsCleared();
}

void MediaSourcePrivate::afterEstablished()
{
    UDOCA::Request req = UserProfile::MakeRequest();
    Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
    connect(ctxt, &Utils::UDOCAContext::responseReceived,
            this, [this](const UDOCA::Request &req, const UDOCA::Response &resp) { onGetProfileResponseReceived(req, resp); });
    m_communicator.sendRequest(req, ctxt);
    // m_mediaSourceStatus = MediaSource::StatusOnline;
}

} // namespace Internal
} // namespace ClearVision
