#include <QCoreApplication>

#include <resource/resource.h>
#include <share/Constants.h>
#include <utils/tmplog.h>

#include "clearvisionnodes.h"
#include "clearvisionnodes_p.h"
#include "constants.h"

namespace ClearVision {
namespace Internal {

ObjectNode::~ObjectNode()
{
}

void ObjectNode::fetchingMore()
{
    if (StatusNode *node = statusNode()) {
        node->setStatus(StatusNode::InProcessStatus);
        node->setInfo(QCoreApplication::translate(Constants::CLEARVISION_TR_SCOPE, Constants::TR_UNITINFO_REQUESTING));
    }
}

void ObjectNode::fetchMoreSucceed()
{
    if (StatusNode *node = statusNode()) {
        node->setStatus(StatusNode::SucceedStatus);
    }
}

void ObjectNode::fetchMoreFailed(const QString &error)
{
    if (StatusNode *node = statusNode()) {
        node->setStatus(StatusNode::FailedStatus);
        node->setInfo(QCoreApplication::translate(Constants::CLEARVISION_TR_SCOPE, Constants::TR_UNITINFO_FAILED1).arg(error));
    }
}

bool ObjectNode::isCamera() const
{
    return false;
}

StatusNode *ObjectNode::statusNode() const
{
    StatusNode *node = 0;
    if (!isCamera()) {
        node = dynamic_cast<StatusNode *>(childNodes().first());
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////////

/*
 * camera root node in camera tree
 */

RootNode::RootNode()
{
    StatusNode *node = new StatusNode();
    addNode(node);
}

Core::Id RootNode::id() const
{
    return Constants::CLEARVISION_OBJECT_ROOTID;
}

QString RootNode::displayName() const
{
    return QCoreApplication::translate(Constants::CLEARVISION_TR_SCOPE, Constants::TR_CAMERAS);
}

QIcon RootNode::icon() const
{
    return Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, Resource::Constants::IDX_HOME);
}

Core::Id RootNode::unitId() const
{
    return Constants::CLEARVISION_OBJECT_ROOTID;
}

////////////////////////////////////////////////////////////////////////////////

StatusNode::StatusNode(StatusNode::Status st)
    : m_status(st)
{
}

bool StatusNode::showInTree() const
{
    TM_DBG << (m_status & VisibleMask);
    return (m_status & VisibleMask);
}

void StatusNode::setStatus(StatusNode::Status st)
{
    if (m_status == st)
        return;

    emitNodeHiddenStateAboutToChange();
    m_status = st;
    emitNodeHiddenStateChanged();
    // TODO: status
}

void StatusNode::setInfo(const QString &info)
{
    if (m_info == info)
        return;

    emitNodeSortKeyAboutToChange();
    m_info = info;
    emitNodeSortKeyChanged();
    emitNodeUpdated();
}

Core::Id StatusNode::id() const
{
    return (parentFolderNode()? parentFolderNode()->id(): Core::Id());
}

QString StatusNode::displayName() const
{
    return m_info;
}

QIcon StatusNode::icon() const
{
    int idx = 0;
    switch (m_status) {
    case InProcessStatus:
        idx = Resource::Constants::IDX_INFOMATION;
        break;
    case FailedStatus:
        idx = Resource::Constants::IDX_ERROR;
        break;
    default:
        idx = -1;
        break;
    }
    return ((idx == -1)? QPixmap(): Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, idx));
}

} // namespace Internal
} // namespace ClearVision
