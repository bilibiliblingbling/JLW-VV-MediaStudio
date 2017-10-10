#include <utils/algorithm.h>
#include <utils/tmplog.h>

#include "basicnodes.h"
#include "basictreemanager.h"
#include "metadata.h"


namespace ProjectExplorer {

/*!

    Node -> Model -> View -> TreeWidget -> Widget -> Holder
     ^^
  MetaData

 */

/*!
 * \brief The Node class
 *  需要在 tree widget 中展示的元素数据,
 */

Node::Node() : m_folderNode(0)
{
}

Node::~Node()
{
}

/*!
 * \brief id
 * \return 节点的 id
 *  在每个工程中应该唯一
 */
Core::Id Node::id() const
{
    return (metadata() ? metadata()->id(): Core::Id());
}

/*!
 * \brief displayName
 * \return 在 tree item 中需要显示的名称
 */
QString Node::displayName() const
{
    return id().toString();
}

/*!
 * \brief tooltip
 * \return 在 tree item 中需要显示的 tooltip
 */
QString Node::tooltip() const
{
    return displayName();
}

/*!
 * \brief enabled
 * \return 是否启用, true by default
 *  会影响 tree item 中的图标或字体
 */
bool Node::enabled() const
{
    return true;
}

/*!
 * \brief icon
 * \return 在 tree item 中显示的 icon
 */
QIcon Node::icon() const
{
    return QIcon();
}

MetaData *Node::metadata() const
{
    return 0;
}

/*!
 * \brief isValid
 * \return
 */
bool Node::isValid() const
{
    return id().isValid();
}

FolderNode *Node::parentFolderNode() const
{
    return m_folderNode;
}

/*!
 * \brief Node::priority
 * \return the display priority in tree view
 */
qint32 Node::priority() const
{
    return -1;
}

void Node::emitNodeUpdated()
{
    if (parentFolderNode()) {
        // emit only when parent folder not empty
        Internal::BasicTreeManager::emitNodeUpdated(this);
    }
}

void Node::setParentFolderNode(FolderNode *folder)
{
    m_folderNode = folder;
}

void Node::emitNodeHiddenStateAboutToChange()
{
    if (parentFolderNode())
        Internal::BasicTreeManager::emitNodeHiddenStateAboutToChange(this);
}

void Node::emitNodeHiddenStateChanged()
{
    if (parentFolderNode())
        Internal::BasicTreeManager::emitNodeHiddenStateChanged(this);
}

void Node::emitNodeSortKeyAboutToChange()
{
    if (parentFolderNode())
        Internal::BasicTreeManager::emitNodeSortKeyAboutToChange(this);
}

void Node::emitNodeSortKeyChanged()
{
    if (parentFolderNode())
        Internal::BasicTreeManager::emitNodeSortKeyChanged(this);
}


////////////////////////////////////////////////////////////////////////////////

namespace {

/*!
 * \brief addToSortedNodes
 * \param dst
 * \param src_
 *  add \p src_ to \p dst
 */
template<typename T>
void addToSortedNodes(T &dst, const T &src_)
{
    T src = src_;
    Utils::sort(src, sortNodes);

    typedef typename T::iterator Iterator;

    if (dst.empty() || sortNodes(dst.last(), src.first())) {
        dst.append(src);
        return;
    }

    Iterator dBgnTt = dst.begin();
    Iterator dEndIt = dst.end();
    Iterator sBgnIt = src.begin();
    Iterator sEndIt = src.end();

    for (; sBgnIt != sEndIt; ++ sBgnIt) {
        dBgnTt = qLowerBound(dBgnTt, dEndIt, *sBgnIt, sortNodes);
        if (dBgnTt == dEndIt) {
            for (; sBgnIt != sEndIt; ++sBgnIt) {
                dst.append(*sBgnIt);
            }
            break;
        }

        dBgnTt = dst.insert(dBgnTt, *sBgnIt);
    }
}

/*!
 * \brief subtractFromSortedNodes
 * \param large
 * \param small_
 *  subtract \p small_ from \p large, stale nodes will be deleted
 */
template<typename T>
void subtractFromSortedNodes(T &large, const T &small_)
{
    typedef typename T::iterator Iterator;
    typedef typename T::value_type ValueType;

    T small = small_;
    Utils::sort(small, sortNodes);

    Iterator lBgnIt = large.begin();
    Iterator lEndIt = large.end();
    Iterator sBgnIt = small.begin();
    Iterator sEndIt = small.end();

    for (; sBgnIt != sEndIt; ++sBgnIt) {
        lBgnIt = Utils::findOr(lBgnIt, lEndIt, [sBgnIt](ValueType v) -> bool { return (v == *sBgnIt); });
        Q_ASSERT(lBgnIt != lEndIt);
        delete *lBgnIt;
        lBgnIt = large.erase(lBgnIt);
    }
}

} // namespace anonymouse

FolderNode::FolderNode()
{
}

FolderNode::~FolderNode()
{
    qDeleteAll(m_subFolderNodes);
    qDeleteAll(m_childNodes);
}

void FolderNode::addSubFolderNode(FolderNode *subFolder)
{
    Q_ASSERT(this != subFolder);

    Internal::BasicTreeManager::emitFolderAboutToBeAdded(this, subFolder);

    subFolder->setParentFolderNode(this);
    if (m_subFolderNodes.empty() || sortNodes(m_subFolderNodes.last(), subFolder)) {
        m_subFolderNodes.append(subFolder);
    } else {
        QList<FolderNode *>::iterator it
                = qLowerBound(m_subFolderNodes.begin(), m_subFolderNodes.end(),
                              subFolder, sortNodes);
        m_subFolderNodes.insert(it, subFolder);
    }

    Internal::BasicTreeManager::emitFolderAdded(this);
}

void FolderNode::removeSubFolderNode(FolderNode *subFolder)
{
    Internal::BasicTreeManager::emitFolderAboutToBeRemoved(this, subFolder);

    QList<FolderNode *>::iterator it = m_subFolderNodes.begin();
    for (; it != m_subFolderNodes.end(); ++it) {
        if (subFolder == *it) {
            delete *it;
            m_subFolderNodes.erase(it);
            break;
        }
    }

    Internal::BasicTreeManager::emitFolderRemoved(this);
}

void FolderNode::addNode(Node *node)
{
    Internal::BasicTreeManager::emitNodeAboutToBeAdded(this, node);

    if (node->parentFolderNode() && node->parentFolderNode() != this) {
        TM_DBG("node(%s) has already a parent folder(%p)",
               node->displayName().toStdString().c_str(),
               node->parentFolderNode());
    }
    node->setParentFolderNode(this);

    if (m_childNodes.empty() || sortNodes(m_childNodes.last(), node)) {
        m_childNodes.append(node);
    } else {
        QList<Node *>::iterator it
                = qLowerBound(m_childNodes.begin(), m_childNodes.end(),
                              node, sortNodes);
        m_childNodes.insert(it, node);
    }

    Internal::BasicTreeManager::emitNodeAdded(this);
}

void FolderNode::removeNode(Node *node)
{
    Internal::BasicTreeManager::emitNodeAboutToBeRemoved(this, node);

    // TODO
    QList<Node *>::iterator it = m_childNodes.begin();
    for (; it != m_childNodes.end(); ++it) {
        if (node == *it) {
            delete *it;
            m_childNodes.erase(it);
            break;
        }
    }

    Internal::BasicTreeManager::emitNodeRemoved(this);
}

void FolderNode::addSubFolderNodes(const QList<FolderNode *> &folders)
{
    if (folders.empty())
        return;

    Internal::BasicTreeManager::emitFoldersAboutToBeAdded(this, folders);

    addToSortedNodes(m_subFolderNodes, folders);

    Internal::BasicTreeManager::emitFoldersAdded(this);
}

/*!
 * \brief FolderNode::removeSubFolderNodes
 * \param folders
 *  all \p folders must be in \p m_subFolderNodes;
 */
void FolderNode::removeSubFolderNodes(const QList<FolderNode *> &folders)
{
    if (folders.empty())
        return;

#if 0
    if (m_subFolderNodes.size() == folders.size()) {
        removeAllSubFolderNodes();
        return;
    }
#endif

    Internal::BasicTreeManager::emitFoldersAboutToBeRemoved(this, folders);

    subtractFromSortedNodes(m_subFolderNodes, folders);

    Internal::BasicTreeManager::emitFoldersRemoved(this);
}

void FolderNode::addNodes(const QList<Node *> &nodes)
{
    if (nodes.empty())
        return;

    Internal::BasicTreeManager::emitNodesAboutToBeAdded(this, nodes);

    addToSortedNodes(m_childNodes, nodes);

    Internal::BasicTreeManager::emitNodesAdded(this);
}

void FolderNode::removeNodes(const QList<Node *> &nodes)
{
    // TODO: remove nodes
    if (nodes.empty())
        return;

#if 0
    if (m_childNodes.size() == nodes.size()) {
        removeAllNodes();
        return;
    }
#endif

    Internal::BasicTreeManager::emitNodesAboutTobeRemoved(this, nodes);

    subtractFromSortedNodes(m_childNodes, nodes);

    Internal::BasicTreeManager::emitNodesRemoved(this);
}

void FolderNode::removeAllSubFolderNodes()
{
    if (m_subFolderNodes.empty())
        return;

    Internal::BasicTreeManager::emitFoldersAboutToBeRemoved(this, m_subFolderNodes);
    qDeleteAll(m_subFolderNodes);
    m_subFolderNodes.clear();
    Internal::BasicTreeManager::emitFoldersRemoved(this);
}

void FolderNode::removeAllNodes()
{
    if (m_childNodes.empty())
        return;

    Internal::BasicTreeManager::emitNodesAboutTobeRemoved(this, m_childNodes);
    qDeleteAll(m_childNodes);
    m_childNodes.clear();
    Internal::BasicTreeManager::emitNodesRemoved(this);
}

QList<FolderNode *> FolderNode::subFolderNodes() const
{
    return m_subFolderNodes;
}

QList<Node *> FolderNode::childNodes() const
{
    return m_childNodes;
}


////////////////////////////////////////////////////////////////////////////////


VirtualFolderNode::VirtualFolderNode(const Core::Id &id, const QString &name)
    : m_id(id), m_displayName(name)
{
}

VirtualFolderNode::~VirtualFolderNode()
{
}

Core::Id VirtualFolderNode::id() const
{
    return m_id;
}

QString VirtualFolderNode::displayName() const
{
    return m_displayName;
}

QIcon VirtualFolderNode::icon() const
{
    return m_icon;
}

void VirtualFolderNode::setIcon(const QIcon &icon)
{
    m_icon = icon;
    emitNodeUpdated();
}

void VirtualFolderNode::setDisplayName(const QString &name)
{
    if (m_displayName == name)
        return;

    emitNodeSortKeyAboutToChange();
    m_displayName = name;
    emitNodeSortKeyChanged();
    emitNodeUpdated();
}

////////////////////////////////////////////////////////////////////////////////

bool sortNodes(Node *n1, Node *n2)
{
    return ((n1->priority() < n2->priority())
            || ((n1->priority() == n2->priority())
                && (n1->displayName() < n2->displayName())));
}

} // namespace ProjectExplorer
