#include <QMutex>
#include <QString>

#include <UDOCA/Protocol.h>

#include <share/AuthenticationHelper.h>

#include "utils/tmplog.h"
#include "udocacommunicator.h"


////////////////////////////////////////////////////////////////////////////////

namespace Utils {

const char KV_MS_PRESET_GUID[]  = "B64BA3DA-EA65-43FE-8C40-243C4A35AA6B";

// TODO: 得改
typedef enum CommunicatorState {
    S_Mask_CanEstablish     = 0x00F0,   // 可以建立会话, 无先前建立会话请求
    S_Mask_InProgress       = 0x0F00,   // 建立会话中
    S_Mask_Ignore           = 0xF000,   // 忽略随后的建立会话响应信息

    S_Disconnected          = 0x00F0,   // 未连接, 初始状态
    S_Canceled              = 0xF0F1,   // 连接请求被取消, 此时应中止后续操作
    S_Terminated            = 0xF0F2,   // 连接已被中止, 如有后续会话建立操作, 应忽略

    S_InProgress            = 0x0F00,   // 上一个建立会话请求已经提交
    S_Connecting            = 0x0F01,   // 正在连接
    S_Connected             = 0x0F02,   // 已连接
    S_Establishing          = 0x0F03,   // 正在认证, 建立会话
    S_Established           = 0x0F04,   // 认证成功, 建立会话成功
} CommunicatorState;

namespace Internal {

class CommunicatorPrivate : public UDOCA::ICommunicatorCallback
{
public:
    CommunicatorPrivate(const char *role, CommunicatorCallback &callback);
    ~CommunicatorPrivate();

    //
public:
    void init();

    // ICommunicatorCallback interface
public:
    /*!
     * \brief onConnecting
     * \param remoteEndpoint
     * \param estCtxt
     */
    void onConnecting(const UDOCA::Endpoint &remoteEndpoint, UDOCA::IContext *estCtxt);
    void onConnectSuccess(const UDOCA::Endpoint &remoteEndpoint, const UDOCA::Endpoint &localEndpoint, UDOCA::IContext *estCtxt);
    void onConnectError(const UDOCA::Exception &e, const UDOCA::Endpoint &remoteEndpoint, UDOCA::IContext *estCtxt);

    void onEstablishResponse(UDOCA::EstablishResponse resp, UDOCA::EstablishRequest &req, UDOCA::EstablishContext ctxt, UDOCA::IContext *estCtxt);
    void onEstablishSuccess(UDOCA::Session session, UDOCA::IContext *estCtxt);
    void onEstablishError(const UDOCA::Exception &e, const UDOCA::Endpoint &remoteEndpoint, UDOCA::IContext *estCtxt);

    /*!
     * 服务器端回调
     *  @{
     */
    void onNewConnection(const UDOCA::Endpoint &, const UDOCA::Endpoint &, bool &) {}
    void onEstablishRequest(UDOCA::EstablishRequest, UDOCA::EstablishResponse &, UDOCA::EstablishContext) {}
    void onNewSession(UDOCA::Session) {}
    /*!
     *  @}
     */

    void onRequestError(const UDOCA::Exception &e, UDOCA::Request req, UDOCA::IContext *reqCtxt, UDOCA::Session session);
    void onReceivedResponse(UDOCA::Response resp, UDOCA::Request req, UDOCA::IContext *reqCtxt, UDOCA::Session session);
    void onReceivedStaleResponse(UDOCA::Response resp, UDOCA::Session session);

    void onReceivedRequest(UDOCA::Request req, UDOCA::Response &resp, UDOCA::Session session);
    void onInvalidMessage(const UDOCA::String &msg, const UDOCA::Exception &e, UDOCA::Session session);

    void onConnectionClosed(const UDOCA::Endpoint &remoteEndpoint, const UDOCA::Endpoint &localEndpoint, UDOCA::Session session);
    void onSessionTerminated(const UDOCA::TerminateReason &reason, UDOCA::IContext *termCtxt, UDOCA::Session session);
    void onSessionDestroy(UDOCA::IContext *) {}
    void onReceivedProvisionalResponse(UDOCA::Request, UDOCA::IContext *, UDOCA::Session) {}

public:
    mutable QMutex m_lock;
    CommunicatorState m_state;

public:
    UDOCA::String m_role;
    UDOCA::Communicator m_comm;
    AuthenticationHelper m_authHelper;

    UDOCA::Session m_session;

public:
    CommunicatorCallback &m_callback;
};


CommunicatorPrivate::CommunicatorPrivate(const char *role, CommunicatorCallback &callback)
    : m_lock(QMutex::Recursive)
    , m_state(S_Disconnected)
    , m_role(role)
    , m_callback(callback)
{
}

CommunicatorPrivate::~CommunicatorPrivate()
{
    QMutexLocker locker(&m_lock);

    if (m_session.isValid())
        m_session.terminate(UDOCA::TerminateReason(UDOCA_TERMINATE_REASON_SHUTDOWN));

    if (m_comm.isValid())
        m_comm.shutdown();

    m_comm.resetImpl();
    m_session.resetImpl();

    // FIXME: exit-crash
}

void CommunicatorPrivate::init()
{
    m_comm = UDOCA::Communicator::create(UDOCA::ObjectIdentity(UDOCA::String(KV_MS_PRESET_GUID), m_role), *this);
    Q_ASSERT_X(m_comm.isValid(), __FUNC_NAME__, "failed to create UDOCA::Communicator");
}

void CommunicatorPrivate::onConnecting(const UDOCA::Endpoint &remoteEndpoint, UDOCA::IContext *estCtxt)
{
    Q_UNUSED(estCtxt);

    QMutexLocker locker(&m_lock);

    if (m_state & S_Mask_Ignore)
        return;

    m_state = S_Connecting;

    m_callback.onConnecting(remoteEndpoint);
}

void CommunicatorPrivate::onConnectSuccess(const UDOCA::Endpoint &remoteEndpoint, const UDOCA::Endpoint &localEndpoint, UDOCA::IContext *estCtxt)
{
    Q_UNUSED(estCtxt);

    QMutexLocker locker(&m_lock);

    if (m_state & S_Mask_Ignore)
        return;

    m_state = S_Connected;

    m_callback.onConnected(localEndpoint, remoteEndpoint);
}

void CommunicatorPrivate::onConnectError(const UDOCA::Exception &e, const UDOCA::Endpoint &remoteEndpoint, UDOCA::IContext *estCtxt)
{
    Q_UNUSED(estCtxt);

    QMutexLocker locker(&m_lock);

    if (m_state & S_Mask_Ignore)
        return;

    m_state = S_Disconnected;

    m_callback.onConnectFailed(remoteEndpoint, e);
}

void CommunicatorPrivate::onEstablishResponse(UDOCA::EstablishResponse resp, UDOCA::EstablishRequest &req, UDOCA::EstablishContext ctxt, UDOCA::IContext *estCtxt)
{
    TM_MARK;

    Q_UNUSED(estCtxt);

    QMutexLocker locker(&m_lock);

    if (m_state & S_Mask_Ignore)
        return;

    m_state = S_Establishing;

    m_authHelper.ProcessEstablishResponse(resp, req, ctxt);
}

void CommunicatorPrivate::onEstablishSuccess(UDOCA::Session session, UDOCA::IContext *estCtxt)
{
    TM_MARK;

    Q_UNUSED(estCtxt);

    Q_ASSERT_X(session.isValid() && session.isEstablished(), __FUNC_NAME__, "call establish success with invalid-unestablished session");

    QMutexLocker locker(&m_lock);

    if (m_state & S_Mask_Ignore)
        return;

    m_session = session;
    m_state = S_Established;

    m_callback.onEstablished();
}

void CommunicatorPrivate::onEstablishError(const UDOCA::Exception &e, const UDOCA::Endpoint &remoteEndpoint, UDOCA::IContext *estCtxt)
{
    Q_UNUSED(estCtxt);

    QMutexLocker locker(&m_lock);

    if (m_state & S_Mask_Ignore)
        return;

    m_session.resetImpl();
    m_state = S_Disconnected;

    m_callback.onEstablishedFailed(remoteEndpoint, e);
}

void CommunicatorPrivate::onRequestError(const UDOCA::Exception &e, UDOCA::Request req, UDOCA::IContext *reqCtxt, UDOCA::Session session)
{
    Q_ASSERT(session == m_session);
    m_callback.onResponseReceived(req, req.createResponse(e), reqCtxt);
}

void CommunicatorPrivate::onReceivedResponse(UDOCA::Response resp, UDOCA::Request req, UDOCA::IContext *reqCtxt, UDOCA::Session session)
{
    Q_ASSERT(session == m_session);
    m_callback.onResponseReceived(req, resp, reqCtxt);
}

void CommunicatorPrivate::onReceivedStaleResponse(UDOCA::Response resp, UDOCA::Session session)
{
    Q_ASSERT(session == m_session);
    TM_DBG << "stale response:" << resp.toXML().getData();
}

void CommunicatorPrivate::onReceivedRequest(UDOCA::Request req, UDOCA::Response &resp, UDOCA::Session session)
{
    Q_ASSERT(session == m_session);

    // 对于客户端来说, 都是通知请求
    m_callback.onRequestReceived(req);

    resp = req.createResponse();
}

void CommunicatorPrivate::onInvalidMessage(const UDOCA::String &msg, const UDOCA::Exception &e, UDOCA::Session session)
{
    Q_ASSERT(session == m_session);
    TM_DBG << "invalid message:" << msg.getData() << "err:" << e.getMessage().getData();
}

void CommunicatorPrivate::onConnectionClosed(const UDOCA::Endpoint &remoteEndpoint, const UDOCA::Endpoint &localEndpoint, UDOCA::Session session)
{
    Q_ASSERT(session == m_session);
    TM_DBG << "connection("
           << localEndpoint.toUrl().getData() << "->" << remoteEndpoint.toUrl().getData()
           << ") closed";
}

void CommunicatorPrivate::onSessionTerminated(const UDOCA::TerminateReason &reason, UDOCA::IContext *termCtxt, UDOCA::Session session)
{
    Q_UNUSED(termCtxt);

    if (session != m_session)
        return;

    QMutexLocker locker(&m_lock);

    m_session.resetImpl();

    if (m_state != S_Canceled) {
        m_state = S_Disconnected;
        m_callback.onTerminated(reason);
    }
}

} // namespace Internal


////////////////////////////////////////////////////////////////////////////////


Communicator::Communicator(const char *role, CommunicatorCallback &callback)
    : d(new Internal::CommunicatorPrivate(role, callback))
{
}

Communicator::~Communicator()
{
    delete d;
}

bool Communicator::establish(const UDOCA::Endpoint &peer, const QString &username, const QString &password)
{
    QMutexLocker locker(&d->m_lock);
    if (!(d->m_state & S_Mask_CanEstablish)) {
        TM_DBG("previous task is in queue or already have established");
        return false;
    }

    d->m_state = S_InProgress;

    if (!d->m_comm.isValid()) {
        d->init();
    }

    d->m_authHelper.SetCredential(username.toStdString(), password.toStdString());

    /*
     * 由于一个 Communicator 对应一个 Session, 对应一个建立 Session 请求者,
     * 不再需要使用 ctxt 进行区分性的工作
     */
    d->m_comm.establish(peer, NULL);

    return true;
}

bool Communicator::establish(const UDOCA::Endpoint &peer, const UDOCA::Endpoint &local, const QString &username, const QString &password)
{
    QMutexLocker locker(&d->m_lock);
    if (!(d->m_state & S_Mask_CanEstablish)) {
        TM_DBG("previous task is in queue or already have established");
        return false;
    }

    d->m_state = S_InProgress;

    if (!d->m_comm.isValid()) {
        d->init();
    }

    d->m_authHelper.SetCredential(username.toStdString(), password.toStdString());

    /*
     * 由于一个 Communicator 对应一个 Session, 对应一个建立 Session 请求者,
     * 不再需要使用 ctxt 进行区分性的工作
     */
    d->m_comm.establish(peer, local, UDOCA::EstablishRequest::create(), NULL);

    return true;
}

bool Communicator::terminate()
{
    QMutexLocker locker(&d->m_lock);
    if (!d->m_session.isValid() || !d->m_session.isEstablished()) {
        TM_DBG("terminate future session");
        d->m_state = S_Terminated;
        return true;
    }

    d->m_session.terminate(UDOCA::TerminateReason(UDOCA_TERMINATE_REASON_USER), NULL);

    return true;
}

void Communicator::sendRequest(const UDOCA::Request &req, UDOCA::IContext *ctxt)
{
    QMutexLocker locker(&d->m_lock);
    try {
        if (!d->m_session.isValid() || !d->m_session.isEstablished()) {
            throw UDOCA::SystemException::create(UDOCA_ERROR_SESSION_UNESTABLISHED);
        }

        d->m_session.sendRequest(req, ctxt);
    } catch (const UDOCA::Exception &e) {
        TM_DBG("failed to send request: %s", e.getMessage().getData());
        d->m_callback.onResponseReceived(req, req.createResponse(e), ctxt);
    }
}

////////////////////////////////////////////////////////////////////////////////

CommunicatorCallback::CommunicatorCallback(QObject *parent)
    : QObject(parent)
{
}

CommunicatorCallback::~CommunicatorCallback()
{
}

bool CommunicatorCallback::verifyResponse(const UDOCA::Response &resp, QString *errorInfo)
{
    if (!resp.isValid())
        return false;

    if (resp.isFailure()) {
        TM_DBG << "failure, reason:" << resp.getException().getMessage().getData();
        if (errorInfo) {
            *errorInfo = resp.getException().getMessage().getData();
        }
        return false;
    }

    return true;
}

} // namespace Utils
