#ifndef CLEARVISION_NODES_H
#define CLEARVISION_NODES_H

#include <projectexplorer/basicnodes.h>

namespace ClearVision {
namespace Internal {

namespace PE = ProjectExplorer;

class StatusNode;
class ObjectNode : public ProjectExplorer::FolderNode
{
public:
    virtual ~ObjectNode();

public:
    void fetchingMore();
    void fetchMoreSucceed();
    void fetchMoreFailed(const QString &error);

public:
    virtual bool isCamera() const;
    /*!
     * \brief unitId
     * \return 所属组织单元
     */
    virtual Core::Id unitId() const = 0;

protected:
    StatusNode *statusNode() const;
};


class RootNode : public ObjectNode
{
public:
    RootNode();

public:
    Core::Id id() const Q_DECL_OVERRIDE;
    QString displayName() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;

public:
    Core::Id unitId() const Q_DECL_OVERRIDE;
};


// 请求组织单元状态节点
class StatusNode : public PE::Node
{
public:
    enum Status {
          NoneSatus         = 0x00
        , SucceedStatus     = 0x01

        , InProcessStatus   = 0x10
        , FailedStatus      = 0x11

        , VisibleMask       = 0x10
    };

public:
    StatusNode(Status st = NoneSatus);

public:
    qint32 status() const;
    bool isReady() const;

public:
    bool showInTree() const;
    void setStatus(Status st);
    void setInfo(const QString &info = QString());

public:
    Core::Id id() const Q_DECL_OVERRIDE;
    QString displayName() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;

private:
    qint32 m_status;
    QString m_info;
};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_NODES_H
