#ifndef UTILS_COMM_UDOCA_COMMUNICATOR_H
#define UTILS_COMM_UDOCA_COMMUNICATOR_H

#include <QObject>
#include <QScopedPointer>

#include <UDOCA/Protocol.h>

#include "utils/utils_global.h"

namespace Utils {

/*!
 * \brief The CommunicatorCallback class
 *  简化版的 UDOCA::ICommunicatorCallback
 */
class MEDIASTUDIO_UTILS_EXPORT CommunicatorCallback : public QObject
{
    Q_OBJECT

public:
    CommunicatorCallback(QObject *parent = 0);
    virtual ~CommunicatorCallback();

public:
    /*!
     * \brief onConnecting, onConnected, onConnectFailed
     * \param local, 连接本地信息
     * \param peer, 连接远端信息
     * \param e, 失败原因
     *  连接相关回调, 正在连接/连接成功/连接失败
     */
    virtual void onConnecting(const UDOCA::Endpoint &peer) = 0;
    virtual void onConnected(const UDOCA::Endpoint &local, const UDOCA::Endpoint &peer) = 0;
    virtual void onConnectFailed(const UDOCA::Endpoint &peer, const UDOCA::Exception &e) = 0;

    /*!
     * \brief onEstablished, onEstablishedFailed
     * \param e, 失败原因
     *  建立会话相关回调, 会话建立成功/会话建立失败
     */
    virtual void onEstablished() = 0;
    virtual void onEstablishedFailed(const UDOCA::Endpoint &peer, const UDOCA::Exception &e) = 0;

    /*!
     * \brief onResponseReceived
     * \param req
     * \param resp
     * \param ctxt, 发送请求时传入的上下文, 推荐以 枚举变量值 或者 序号 进行区分, \c Communicator 不负责处理
     *  收到请求响应信息, 需要判断是否成功
     */
    virtual void onResponseReceived(const UDOCA::Request &req, const UDOCA::Response &resp, UDOCA::IContext *ctxt) = 0;

    /*!
     * \brief onRequestReceived
     * \param req
     *  收到请求, 客户端收到的均为通知请求
     */
    virtual void onRequestReceived(const UDOCA::Request &req) = 0;

    /*!
     * \brief onTerminated
     * \param reason
     *  连接断开
     */
    virtual void onTerminated(const UDOCA::TerminateReason &reason) = 0;

protected:
    bool verifyResponse(const UDOCA::Response &resp, QString *errorInfo = 0);
};


namespace Internal { class CommunicatorPrivate; }

class MEDIASTUDIO_UTILS_EXPORT Communicator
{
public:
public:
    Communicator(const char *role, CommunicatorCallback &callback);
    ~Communicator();

public:
    bool establish(const UDOCA::Endpoint &peer, const QString &username, const QString &password);
    bool establish(const UDOCA::Endpoint &peer, const UDOCA::Endpoint &local, const QString &username, const QString &password);

    bool terminate();

    void sendRequest(const UDOCA::Request &req, UDOCA::IContext *ctxt);

    // TODO: more interface required
public:

private:
    Internal::CommunicatorPrivate *d;
};

} // namespace Utils


#endif // UTILS_COMM_UDOCA_COMMUNICATOR_H
