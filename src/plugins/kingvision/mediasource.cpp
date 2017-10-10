#include <QCoreApplication>

#include <mediasource/constants.h>
#include <utils/communicator/udocacommunicator.h>
#include <utils/tmplog.h>

#include "constants.h"
#include "mediasource.h"
#include "mediasource_p.h"

namespace KingVision {
namespace Internal {

namespace MS = MediaSource;

const int  KINGVISION_RECORDTYPE_ALL                = 7;

MediaSource::MediaSource()
    : ProjectExplorer::MediaSource(Core::Id(Constants::MEDIASOURCE_CLASSID))
    , d(new MediaSourcePrivate(this))
{
}

MediaSource::~MediaSource()
{
    qDeleteAll(d->m_codecs);
    qDeleteAll(d->m_cameras);

    qDeleteAll(d->m_segments);

    delete d;
}

QList<Codec *> MediaSource::codecs() const
{
    return d->m_codecs.values();
}

QList<Camera *> MediaSource::cameras() const
{
    return d->m_cameras.values();
}

QString MediaSource::className() const
{
    return QCoreApplication::translate("KingVision", Constants::MEDIASOURCE_CLASSNAME);
}

MediaSource::Status MediaSource::status() const
{
    return d->m_mediaSourceStatus;

#if 0
    if (!enabled())
        return StatusDisabled;

    if (d->m_communicator.isEstablished())
        return StatusOnline;

    if (d->m_communicator.isEstablishing())
        return StatusLoggingIn;

    return StatusLoggingIn;
#endif
}

void MediaSource::checkStatus()
{
    if (d->m_mediaSourceStatus == StatusOffline) {
        d->m_mediaSourceStatus = StatusLoggingIn;
        UDOCA::Endpoint ep("http", d->m_storageProxyAddr.toStdString(), d->m_storageProxyPort);
        d->m_communicator.establish(ep, QLatin1String(""), QLatin1String(""));
        return;
    }

    if (d->m_mediaSourceStatus == StatusOnline) {
        // check info
    }
}

void MediaSource::startRecording(Core::Id cameraId)
{
    d->startRecording(cameraId);
}

void MediaSource::stopRecording(Core::Id cameraId)
{
    d->stopRecording(cameraId);
}

void MediaSource::startRecordingAll()
{
    d->startRecordingAll();
}

void MediaSource::stopRecordingAll()
{
    d->stopRecordingAll();
}

/*!
 * \brief MediaSource::queryRecordings
 * \param conditions
 *  根据条件查询录像片段
 */
void MediaSource::queryRecordings(const QVariantMap &conditions)
{
    d->queryRecordings(conditions);
}

QVariantMap MediaSource::lastQueryConditions() const
{
    return d->m_lastQueryConditions;
}

QList<PE::Segment *> MediaSource::segments() const
{
    return d->m_segments.values();
}

Camera *MediaSource::cameraOfId(Core::Id id) const
{
    return d->m_cameras.value(id);
}

void MediaSource::clearCameraSegments(Core::Id id)
{
    PE::Camera *camera = d->m_cameras.value(id);
    if (camera) {
        emit cameraSegmentsAboutToBeCleared(camera);

        QList<Core::Id> segments;
        foreach (auto seg, d->m_segments) {
            if (seg->cameraId() == id) {
                segments.push_back(seg->infoId());
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
        if (seg->id() == id) {
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

bool MediaSource::fromMap(const QVariantMap &map)
{
    if (!ProjectExplorer::MediaSource::fromMap(map))
        return false;

    QVariantMap arguments = map.value(QLatin1String(Constants::MEDIASOURCE_ARGUMENTS)).toMap();
    d->m_storageProxyAddr = arguments.value(QLatin1String(Constants::MEDIASOURCE_STORAGEPROXYADDR)).toString();
    d->m_storageProxyPort = arguments.value(QLatin1String(Constants::MEDIASOURCE_STORAGEPROXYPORT)).toUInt();
    if (!d->m_storageProxyPort)
        d->m_storageProxyPort = Constants::MEDIASOURCE_STORAGEPROXYPORT_DEFAULT;

    return true;
}

QVariantMap MediaSource::toMap() const
{
    QVariantMap map(ProjectExplorer::MediaSource::toMap());

    QVariantMap arguments;
    arguments.insert(QLatin1String(Constants::MEDIASOURCE_STORAGEPROXYADDR), d->m_storageProxyAddr);
    arguments.insert(QLatin1String(Constants::MEDIASOURCE_STORAGEPROXYPORT), d->m_storageProxyPort);

    map.insert(QLatin1String(Constants::MEDIASOURCE_ARGUMENTS), arguments);

    return map;
}


////////////////////////////////////////////////////////////////////////////////


MediaSourcePrivate::MediaSourcePrivate(MediaSource *source)
    : CommunicatorCallback(source)
    , m_communicator(Constants::MEDIASOURCE_KINGVISION_ROLE, *this)
    , m_mediasource(source)
    , m_mediaSourceStatus(MediaSource::StatusOffline)
    , m_storageProxyPort(0)
    , m_liveStreamPort(0)
    , m_recordStreamPort(0)
    , m_deviceLock(QMutex::Recursive)
    , m_queryLock(QMutex::Recursive)
{
    UDOCA::String::setLocalCharset(UDOCA::String::CHARSET_UTF8);

    connect(this, &MediaSourcePrivate::established, this, [this]() { afterEstablished(); });
    connect(this, &MediaSourcePrivate::serviceConfigGot, this, [this]() { afterGetServiceConfig(); });
}

MediaSourcePrivate::~MediaSourcePrivate()
{
}

void MediaSourcePrivate::startRecording(Core::Id cameraId)
{
    if (cameraId.isValid()) {
        UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
        req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
        req.setMethodName("StartRecord");
        req.setParameter("CameraID", UDOCA::String(cameraId.toString().toStdString()));
        req.setParameter("Duration", 0);

        Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);

        connect(ctxt, &Utils::UDOCAContext::responseReceived,
                this, [this, cameraId](const UDOCA::Request &req, const UDOCA::Response &resp) { onStartRecordResponseReceived(cameraId, req, resp); });

        m_communicator.sendRequest(req, ctxt);
    } else {
        TM_DBG << " currentCameraID not valid " << cameraId.toString().toStdString();
    }
}

void MediaSourcePrivate::stopRecording(Core::Id cameraId)
{
    if (cameraId.isValid()) {
        UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
        req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
        req.setMethodName("StopRecord");
        req.setParameter("CameraID", UDOCA::String(cameraId.toString().toStdString()));
        req.setParameter("Duration", 0);

        Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);

        connect(ctxt, &Utils::UDOCAContext::responseReceived,
                this, [this, cameraId](const UDOCA::Request &req, const UDOCA::Response &resp) { onStopRecordResponseReceived(cameraId, req, resp); });

        m_communicator.sendRequest(req, ctxt);
    } else {
        TM_DBG << " currentCameraID not valid " << cameraId.toString().toStdString();
    }
}

void MediaSourcePrivate::startRecordingAll()
{
    UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
    req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
    req.setMethodName("StartRecordAll");
    req.setParameter("Duration", 0);

    Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);

    connect(ctxt, &Utils::UDOCAContext::responseReceived,
            this, [this](const UDOCA::Request &req, const UDOCA::Response &resp) { onStartRecordAllResponseReceived(req, resp); });

    m_communicator.sendRequest(req, ctxt);
}

void MediaSourcePrivate::stopRecordingAll()
{
    UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
    req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
    req.setMethodName("StopRecordAll");
    req.setParameter("Duration", 0);

    Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);

    connect(ctxt, &Utils::UDOCAContext::responseReceived,
            this, [this](const UDOCA::Request &req, const UDOCA::Response &resp) { onStopRecordAllResponseReceived(req, resp); });

    m_communicator.sendRequest(req, ctxt);
}

void MediaSourcePrivate::queryRecordings(const QVariantMap &conditions)
{
    // TODO:
    bool append = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_APPENDRESULTS)).toBool();
    if (!append) {
        emit m_mediasource->segmentsAboutToBeCleared();
        // TODO: as previous segments will be deleted, the segment used in fast preview or should be deep-copied
        qDeleteAll(m_segments);
        m_segments.clear();
        emit m_mediasource->segmentsCleared();
    }

    QStringList cameraList = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_CAMERALIST)).toStringList();
    quint64 bgnTime = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_BGNTIME)).toULongLong();
    quint64 endTime = conditions.value(QLatin1String(MS::Constants::MEDIASOURCE_QUERY_ENDTIME)).toULongLong();

    UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
    req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
    req.setMethodName("QueryRecordingsByCamera");

    UDOCA::List camlst = UDOCA::List::create();

    if (cameraList.isEmpty()) {
        // the server may contain more cameras than expect
        foreach (auto camera, m_cameras) {
            camlst.add(UDOCA::String(camera->id().toString().toStdString()));
        }
    } else {
        foreach (const QString &camera, cameraList) {
            camlst.add(UDOCA::String(camera.toStdString()));
        }
    }

    req.setParameter("CameraList", camlst);
    req.setParameter("RecordType", KINGVISION_RECORDTYPE_ALL);
    req.setParameter("BeginTime", UDOCA::DateTime(bgnTime));
    req.setParameter("EndTime", UDOCA::DateTime(endTime));

    Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
    connect(ctxt, &Utils::UDOCAContext::responseReceived,
            this, [this, conditions](const UDOCA::Request &req, const UDOCA::Response &resp) { onQueryRecordingsByCameraResponseReceived(conditions, req, resp); });

    m_communicator.sendRequest(req, ctxt);
}

void MediaSourcePrivate::onConnecting(const UDOCA::Endpoint &peer)
{
    // TODO: set blinkable
    Q_UNUSED(peer);
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

    emit established();
}

void MediaSourcePrivate::onEstablishedFailed(const UDOCA::Endpoint &peer, const UDOCA::Exception &e)
{
    TM_DBG("failed to establish connect with (%s), error info: %s",
           peer.toUrl().getData(), e.getMessage().getData());
    // TODO: post error info

    // may need to sleep a while before next estatblishing
    m_mediaSourceStatus = MediaSource::StatusOffline;
}

void MediaSourcePrivate::onResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp, UDOCA::IContext *ctxt)
{
    auto udocactxt = dynamic_cast<Utils::UDOCAContext *>(ctxt);
    Q_ASSERT_X(udocactxt, "onResponseReceived();", "invalid ctxt received");
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
    // TODO:
    //TM_DBG << req.toXML().getData();
}

void MediaSourcePrivate::onTerminated(const UDOCA::TerminateReason &reason)
{
    // TODO:
    TM_DBG << reason.getMessage().getData();
}

void MediaSourcePrivate::onGetServiceConfigResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);

    QString errorInfo;
    Q_ASSERT(verifyResponse(resp, &errorInfo));

    m_mediaSourceStatus = MediaSource::StatusOnline;

    UDOCA::InvokeMethodResponse imresp(resp);
    Q_ASSERT(imresp.isValid());
    UDOCA::Object o = imresp.getReturn();

    try {
        m_liveStreamPort = o.getProperty("LiveStreamPort");
    } catch (const UDOCA::Exception &e) {
        TM_DBG << e.getMessage().getData();
        m_liveStreamPort = 0;
    }

    try {
        m_recordStreamPort = o.getProperty("RecordStreamPort");
    } catch (const UDOCA::Exception &e) {
        TM_DBG << e.getMessage().getData();
        m_recordStreamPort = 0;
    }

    // TODO: handle failure
    emit serviceConfigGot();
}

void MediaSourcePrivate::onGetDeviceListResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);

    Q_ASSERT(verifyResponse(resp, 0));

    UDOCA::InvokeMethodResponse imresp(resp);
    UDOCA::List devices = imresp.getReturn().toList();
    UDOCA::Iterator it = devices.iterator();
    while (it.hasNext()) {
        UDOCA::Object obj = it.next();
        if (!obj.isValid())
            continue;

#ifdef DUMP_OBJECT_TO_XML
        TM_DBG << "---------------------- Codec -------------------------------";
        TM_DBG << UDOCA::ObjectToXML(obj).getData();
#endif

        Codec *codec = Codec::create(obj);
        if (codec && codec->isValid()) {
            Q_ASSERT(!m_codecs.contains(codec->id()));
            emit m_mediasource->codecAboutToBeAdded(codec);
            m_codecs.insert(codec->id(), codec);
            emit m_mediasource->codecAdded();
        }
    }
}

void MediaSourcePrivate::onGetCameraListResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);

    Q_ASSERT(verifyResponse(resp, 0));

    UDOCA::InvokeMethodResponse imresp(resp);
    UDOCA::List devices = imresp.getReturn().toList();
    UDOCA::Iterator it = devices.iterator();
    while (it.hasNext()) {
        UDOCA::Object obj = it.next();
        if (!obj.isValid())
            continue;

#ifdef DUMP_OBJECT_TO_XML
        TM_DBG << "--------------------- Camera -------------------------------";
        TM_DBG << UDOCA::ObjectToXML(obj).getData();
#endif

        Camera *camera = Camera::create(obj);
        if (camera && camera->isValid()) {
            Q_ASSERT(!m_cameras.contains(camera->id()));
            emit m_mediasource->cameraAboutToBeAdded(camera);
            m_cameras.insert(camera->id(), camera);
            emit m_mediasource->cameraAdded();
        }
    }
}

void MediaSourcePrivate::onQueryRecordingsByCameraResponseReceived(const QVariantMap &conditions, const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req);

    if (!verifyResponse(resp, 0))
        return;
    // TODO: handle errors

    QMutexLocker locker(&m_queryLock);

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

void MediaSourcePrivate::onStartRecordResponseReceived(Core::Id cameraId, const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(cameraId)
    Q_UNUSED(req)
    Q_UNUSED(resp)

    /*! @brief 判断发送请求是否成功,通过查看OBJ状态(是否为录像中)
     *  @param CameraID 发送开始录制请求设备的标识
     *  @wxf 我就到这了!!!
     */
}

void MediaSourcePrivate::onStopRecordResponseReceived(Core::Id cameraId, const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(cameraId)
    Q_UNUSED(req)
    Q_UNUSED(resp)

    /*! @brief 判断发送请求是否成功,通过查看OBJ状态(是否为录像中)
     *  @param CameraID 当前发送停止录制请求设备的标识
     *  @wxf 我就到这了!!!
     */
}

void MediaSourcePrivate::onStartRecordAllResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req)
    Q_UNUSED(resp)

    /*! @brief 判断发送请求是否成功
     *  @wxf 我就到这了!!!
     */
}

void MediaSourcePrivate::onStopRecordAllResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp)
{
    Q_UNUSED(req)
    Q_UNUSED(resp)

    /*! @brief 判断发送请求是否成功
     *  @wxf 我就到这了!!!
     */
}

void MediaSourcePrivate::afterEstablished()
{
    UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
    req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
    req.setMethodName("GetServiceConfig");

    Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
    connect(ctxt, &Utils::UDOCAContext::responseReceived,
            this, [this](const UDOCA::Request &req, const UDOCA::Response &resp) { onGetServiceConfigResponseReceived(req, resp); });
    m_communicator.sendRequest(req, ctxt);
}

void MediaSourcePrivate::afterGetServiceConfig()
{
    {
        emit m_mediasource->codecsAboutToBeCleared();
        qDeleteAll(m_codecs);
        m_codecs.clear();
        emit m_mediasource->codecsCleared();

        UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
        req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
        req.setMethodName("GetDeviceList");

        Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
        connect(ctxt, &Utils::UDOCAContext::responseReceived,
                this, [this](const UDOCA::Request &req, const UDOCA::Response &resp) { onGetDeviceListResponseReceived(req, resp); });
        m_communicator.sendRequest(req, ctxt);
    }
    {
        emit m_mediasource->camerasAboutToBeCleared();
        qDeleteAll(m_codecs);
        m_codecs.clear();
        emit m_mediasource->camerasCleared();

        UDOCA::InvokeMethodRequest req = UDOCA::InvokeMethodRequest::create();
        req.setClassName(Constants::KINGVISION_STORAGESERVEREXPRESS);
        req.setMethodName("GetCameraList");

        Utils::UDOCAContext *ctxt = new Utils::UDOCAContext(this);
        connect(ctxt, &Utils::UDOCAContext::responseReceived,
                this, [this](const UDOCA::Request &req, const UDOCA::Response &resp) { onGetCameraListResponseReceived(req, resp); });
        m_communicator.sendRequest(req, ctxt);
    }
}

} // namespace Internal
} // namespace KingVision
