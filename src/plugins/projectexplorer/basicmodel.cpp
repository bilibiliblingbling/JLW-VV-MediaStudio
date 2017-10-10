#include <utils/algorithm.h>
#include <utils/tmplog.h>

#include "basicmodel.h"
#include "basicnodes.h"
#include "basictree.h"
#include "metamimedata.h"

namespace ProjectExplorer {

/*!
 *  \class BasicModel
 *
 */

BasicModel::BasicModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_basictree(0)
    , m_rootNode(0)
    , m_parentNodeForChange(0)
    , m_nodeForSortKeyChange(0)
    , m_nodeForHiddenStateChange(0)
{
}

BasicModel::~BasicModel()
{
}

/*!
 * \brief BasicModel::setBasicTree
 * \param basictree
 *  the ownership of \p basictree is not transfered to this model
 */
void BasicModel::setBasicTree(BasicTree *basictree)
{
    if (basictree == m_basictree)
        return;

    if (m_basictree) {
        TM_DBG << "old basic tree:" << m_basictree->objectName().toStdString();

        disconnect(m_basictree, 0, this, 0);
    }

    m_basictree = basictree;

    if (m_basictree) {
        TM_DBG << "new basic tree:" << m_basictree->objectName().toStdString();

        connect(m_basictree, SIGNAL(nodeUpdated(Node*)),
                this, SLOT(nodeUpdated(Node*)));
        connect(m_basictree, SIGNAL(nodeUpdated(Node*,QVector<int>)),
                this, SLOT(nodeUpdated(Node*,QVector<int>)));

        connect(m_basictree, &BasicTree::folderAboutToBeAdded,
                this, &BasicModel::folderAboutToBeAdded);
        connect(m_basictree, &BasicTree::folderAdded,
                this, &BasicModel::folderAdded);

        connect(m_basictree, &BasicTree::folderAboutToBeRemoved,
                this, &BasicModel::folderAboutToBeRemoved);
        connect(m_basictree, &BasicTree::folderRemoved,
                this, &BasicModel::folderRemoved);

        connect(m_basictree, &BasicTree::nodeAboutToBeAdded,
                this, &BasicModel::nodeAboutToBeAdded);
        connect(m_basictree, &BasicTree::nodeAdded,
                this, &BasicModel::nodeAdded);

        connect(m_basictree, &BasicTree::nodeAboutToBeRemoved,
                this, &BasicModel::nodeAboutToBeRemoved);
        connect(m_basictree, &BasicTree::nodeRemoved,
                this, &BasicModel::nodeRemoved);

        connect(m_basictree, &BasicTree::foldersAboutToBeAdded,
                this, &BasicModel::foldersAboutToBeAdded);
        connect(m_basictree, &BasicTree::foldersAdded,
                this, &BasicModel::foldersAdded);

        connect(m_basictree, &BasicTree::foldersAboutToBeRemoved,
                this, &BasicModel::foldersAboutToBeRemoved);
        connect(m_basictree, &BasicTree::foldersRemoved,
                this, &BasicModel::foldersRemoved);

        connect(m_basictree, &BasicTree::nodeSortKeyAboutToChange,
                this, &BasicModel::nodeSortKeyAboutToChange);
        connect(m_basictree, &BasicTree::nodeSortKeyChanged,
                this, &BasicModel::nodeSortKeyChanged);

        connect(m_basictree, &BasicTree::nodeHiddenStateAboutToChange,
                this, &BasicModel::nodeHiddenStateAboutToChange);
        connect(m_basictree, &BasicTree::nodeHiddenStateChanged,
                this, &BasicModel::nodeHiddenStateChanged);
    }
}

void BasicModel::setRootNode(FolderNode *root)
{
    if (m_rootNode == root)
        return;

    if (m_rootNode) {
        // remove previous data

        foldersAboutToBeRemoved(m_rootNode, m_rootNode->subFolderNodes());

        QModelIndex oldIdx = (m_rootNode? indexForNode(m_rootNode): QModelIndex());
        m_childNodes.remove(m_rootNode);

        if (oldIdx.isValid())
            emit dataChanged(oldIdx, oldIdx);

        TM_DBG << "folder node size after clear:" << m_childNodes.size();
    }

    m_rootNode = root;
    QModelIndex newIdx = (root? indexForNode(root): QModelIndex());

    if (newIdx.isValid())
        emit dataChanged(newIdx, newIdx);
}

QModelIndex BasicModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex rst;

    if (!parent.isValid() && row == 0 && column == 0) {
        rst = createIndex(0, 0, m_rootNode);
    } else if (parent.isValid() && column == 0) {
        FolderNode *parentNode = dynamic_cast<FolderNode *>(nodeForIndex(parent));
        Q_ASSERT(parentNode);
        QHash<FolderNode *, QList<Node *> >::const_iterator it = m_childNodes.find(parentNode);
        if (it == m_childNodes.end()) {
            fetchMore(parentNode);
            it = m_childNodes.find(parentNode);
        }

        if (row < it.value().size()) {
            rst = createIndex(row, 0, it.value().at(row));
        }
    }

    return rst;
}

QModelIndex BasicModel::parent(const QModelIndex &child) const
{
    QModelIndex parentIndex;

    if (Node *node = nodeForIndex(child)) {
        FolderNode *parentNode = visibleFolderNode(node->parentFolderNode());

        if (parentNode) {
            parentIndex = indexForNode(parentNode);
        }
    }

    return parentIndex;
}

QVariant BasicModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (Node *node = nodeForIndex(index)) {
        switch (role) {
        case Qt::DisplayRole: {
            result = node->displayName();
            break;
        }
        case Qt::EditRole: {
            // TODO
            break;
        }
        case Qt::ToolTipRole: {
            result = node->tooltip();
            break;
        }
        case Qt::DecorationRole: {
            result = node->icon();
            break;
        }
        case Qt::FontRole: {
            // TODO
            break;
        }
        case EnabledRole: {
            result = node->enabled();
            break;
        }
        }
    }

    return result;
}

Qt::ItemFlags BasicModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags f = Qt::ItemIsEnabled;
    if (Node *node = nodeForIndex(index)) {
        if (node == m_rootNode)
            return 0;

        if (m_basictree) {
            if (m_basictree->selectableInTree(node))
                f |= Qt::ItemIsSelectable;

            if (m_basictree->renamableInTree(node))
                f |= Qt::ItemIsEditable;

            if (m_basictree->dragableInTree(node))
                f |= Qt::ItemIsDragEnabled;
        }
    }

    return f;
}

bool BasicModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TM_MARK;

    // TODO: update data, especially rename
    return false;
}

int BasicModel::rowCount(const QModelIndex &parent) const
{
    int rows = 0;

    if (!parent.isValid()) {
        rows = 1;
    } else {
        FolderNode *folderNode = nodeForIndex(parent)->asNode<FolderNode>();
        if (folderNode && m_childNodes.contains(folderNode))
            rows = m_childNodes.value(folderNode).size();
    }

    return rows;
}

int BasicModel::columnCount(const QModelIndex &) const
{
    return 1;
}

bool BasicModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return true;

    FolderNode *folderNode = nodeForIndex(parent)->asNode<FolderNode>();
    if (!folderNode)
        return false;

    QHash<FolderNode *, QList<Node *> >::const_iterator it = m_childNodes.find(folderNode);
    if (it == m_childNodes.end()) {
        fetchMore(folderNode);
        it = m_childNodes.find(folderNode);
    }

    return !it.value().isEmpty();
}

bool BasicModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return false;

    if (Node *node = nodeForIndex(parent)) {
        TM_DBG << node->displayName().toStdString().c_str();
        if (FolderNode *folderNode = node->asNode<FolderNode>()) {
            return (!m_childNodes.contains(folderNode)
                    && m_basictree->canFetchMoreInTree(folderNode));
        }
    }

    return false;
}

void BasicModel::fetchMore(const QModelIndex &parent) const
{
    FolderNode *folderNode = nodeForIndex(parent)->asNode<FolderNode>();

    Q_ASSERT(folderNode);

    fetchMore(folderNode);
}

QMimeData *BasicModel::mimeData(const QModelIndexList &indexes) const
{
    Q_ASSERT(indexes.count() == 1);

    MetaMimeData *mmd = new MetaMimeData();
    foreach (const QModelIndex &idx, indexes) {
        Node *n = nodeForIndex(idx);
        if (n && n->metadata()) {
            mmd->addMetaData(n->metadata());
        }
    }
    return mmd;
}

Node *BasicModel::nodeForIndex(const QModelIndex &index) const
{
    if (index.isValid())
        return (Node *)index.internalPointer();

    return 0;
}

QModelIndex BasicModel::indexForNode(const Node *node_) const
{
    Node *node = const_cast<Node *>(node_);
    if (!node)
        return QModelIndex();

    if (node == m_rootNode)
        return createIndex(0, 0, m_rootNode);

    FolderNode *parentNode = visibleFolderNode(node->parentFolderNode());

    // Do we have the parent mapped?
    QHash<FolderNode *, QList<Node *> >::const_iterator it = m_childNodes.find(parentNode);
    if (it == m_childNodes.end()) {
        fetchMore(parentNode);
        it = m_childNodes.find(parentNode);
    }

    if (it != m_childNodes.end()) {
        const int row = it.value().indexOf(node);
        if (row != -1)
            return createIndex(row, 0, node);
    }

    return QModelIndex();
}

Node *BasicModel::nodeForId(Core::Id id) const
{
    if (id.isValid())
        return nodeForId(id, m_rootNode);

    return 0;
}

QModelIndex BasicModel::indexForId(Core::Id id) const
{
    if (!id.isValid())
        return QModelIndex();

    Node *node = nodeForId(id, m_rootNode);
    if (node)
        return indexForNode(node);

    return QModelIndex();
}

void BasicModel::aboutToExpand(const QModelIndex &parent) const
{
    if (Node *node = nodeForIndex(parent)) {
        if (FolderNode *folder = node->asNode<FolderNode>())
            aboutToExpand(folder);
    }
}

bool BasicModel::isRootNode(Node *node) const
{
    return (m_rootNode == node);
}

void BasicModel::added(FolderNode *folder)
{
    if (folder && folder->isValid()) {
        FolderNode *folderNode = visibleFolderNode(folder);
        QList<Node *> newChildList = childNodes(folderNode);
        added(folderNode, newChildList);
    }
}

/*!
 * \brief BasicModel::added
 * \param parentFolder
 * \param newNodeList, all child nodes of \p parentFolder
 *  merge child node list
 */
void BasicModel::added(FolderNode *parentFolder, const QList<Node *> &newNodeList)
{
    QModelIndex parentIndex = indexForNode(parentFolder);

    if (newNodeList.isEmpty())
        return;

    QHash<FolderNode *, QList<Node *> >::const_iterator it = m_childNodes.find(parentFolder);
    if (it == m_childNodes.end()) {
        if (!parentIndex.isValid()) {
            return;
        }
        beginInsertRows(parentIndex, 0, newNodeList.size() - 1);
        m_childNodes.insert(parentFolder, newNodeList);
        endInsertRows();
        return;
    }

    //
    QList<Node *> oldNodeList = it.value();

    // Compare lists and emit signals, and modify m_childNodes on the fly
    QList<Node *>::const_iterator oldIter = oldNodeList.begin();
    QList<Node *>::const_iterator newIter = newNodeList.begin();

    Q_ASSERT(isSorted(oldNodeList, sortNodes));
    Q_ASSERT(isSorted(newNodeList, sortNodes));

    QSet<Node *> emptyDifference;
    emptyDifference = oldNodeList.toSet();
    emptyDifference.subtract(newNodeList.toSet());
    if (!emptyDifference.isEmpty()) {
        // This should not happen...
        TM_DBG << "all old node list should be a subset of new node list";
        foreach (Node *n, emptyDifference) {
            TM_DBG << n->displayName().toStdString();
        }
        Q_ASSERT(0);
    }

    // optimization, check for old list is empty
    if (oldIter == oldNodeList.end()) {
        // New Node List is empty, nothing to add
        if (newIter == newNodeList.end())
            return;

        // So all we need to do is easy
        beginInsertRows(parentIndex, 0, newNodeList.size() - 1);
        m_childNodes.insert(parentFolder, newNodeList);
        endInsertRows();
        return;
    }

    while (true) {
        // Skip all that are the same
        while (*oldIter == *newIter) {
            ++oldIter;
            ++newIter;

            // optimization, check for old list is empty
            if (oldIter == oldNodeList.end()) {
                // At end of oldNodeList, sweep up rest of newNodeList
                QList<Node *>::const_iterator startOfBlock = newIter;
                newIter = newNodeList.end();
                int pos = oldIter - oldNodeList.begin();
                int count = newIter - startOfBlock;
                if (count > 0) {
                    beginInsertRows(parentIndex, pos, pos+count-1);
                    while (startOfBlock != newIter) {
                        oldNodeList.insert(pos, *startOfBlock);
                        ++pos;
                        ++startOfBlock;
                    }
                    m_childNodes.insert(parentFolder, oldNodeList);
                    endInsertRows();
                }
                return; // Done with the lists, leave the function
            }
        }

        QList<Node *>::const_iterator startOfBlock = newIter;
        while (*oldIter != *newIter)
            ++newIter;
        // startOfBlock is the first that was different
        // newIter points to the new position of oldIter
        // newIter - startOfBlock is number of new items
        // oldIter is the position where those are...
        int pos = oldIter - oldNodeList.begin();
        int count = newIter - startOfBlock;
        beginInsertRows(parentIndex, pos, pos + count - 1);
        while (startOfBlock != newIter) {
            oldNodeList.insert(pos, *startOfBlock);
            ++pos;
            ++startOfBlock;
        }
        m_childNodes.insert(parentFolder, oldNodeList);
        endInsertRows();
        oldIter = oldNodeList.begin() + pos;
    }
}

void BasicModel::removed(FolderNode *parentFolder, const QList<Node *> &newNodeList)
{
    QModelIndex parentIndex = indexForNode(parentFolder);
    // Old  list
    QHash<FolderNode*, QList<Node*> >::const_iterator it = m_childNodes.find(parentFolder);
    if (it == m_childNodes.end()) {
        return;
    }

    QList<Node *> oldNodeList = it.value();
    // Compare lists and emit signals, and modify m_childNodes on the fly
    QList<Node *>::const_iterator oldIter = oldNodeList.begin();
    QList<Node *>::const_iterator newIter = newNodeList.begin();

    Q_ASSERT(isSorted(newNodeList, sortNodes));

    QSet<Node *> emptyDifference;
    emptyDifference = newNodeList.toSet();
    emptyDifference.subtract(oldNodeList.toSet());
    if (!emptyDifference.isEmpty()) {
        // This should not happen...
        TM_DBG << "all old node list should be a subset of new node list";
        foreach (Node *n, emptyDifference) {
            TM_DBG << n->displayName().toStdString();
        }
        Q_ASSERT(false);
    }

    // optimization, check for new list is empty
    if (newIter == newNodeList.end()) {
        // New Node List is empty, everything removed
        if (oldIter == oldNodeList.end())
            return;
        // So all we need to do is easy
        beginRemoveRows(parentIndex, 0, oldNodeList.size() - 1);
        m_childNodes.insert(parentFolder, newNodeList);
        endRemoveRows();
        return;
    }

    while (true) {
        // Skip all that are the same
        while (*oldIter == *newIter) {
            ++oldIter;
            ++newIter;
            if (newIter == newNodeList.end()) {
                // At end of newNodeList, sweep up rest of oldNodeList
                QList<Node *>::const_iterator startOfBlock = oldIter;
                oldIter = oldNodeList.end();
                int pos = startOfBlock - oldNodeList.begin();
                int count = oldIter - startOfBlock;
                if (count > 0) {
                    beginRemoveRows(parentIndex, pos, pos+count-1);
                    while (startOfBlock != oldIter) {
                        ++startOfBlock;
                        oldNodeList.removeAt(pos);
                    }

                    m_childNodes.insert(parentFolder, oldNodeList);
                    endRemoveRows();
                }
                return; // Done with the lists, leave the function
            }
        }

        QList<Node *>::const_iterator startOfBlock = oldIter;
        while (*oldIter != *newIter)
            ++oldIter;
        // startOfBlock is the first that was diffrent
        // oldIter points to the new position of newIter
        // oldIter - startOfBlock is number of new items
        // newIter is the position where those are...
        int pos = startOfBlock - oldNodeList.begin();
        int count = oldIter - startOfBlock;
        beginRemoveRows(parentIndex, pos, pos + count - 1);
        while (startOfBlock != oldIter) {
            ++startOfBlock;
            oldNodeList.removeAt(pos);
        }
        m_childNodes.insert(parentFolder, oldNodeList);
        endRemoveRows();
        oldIter = oldNodeList.begin() + pos;
    }
}

void BasicModel::removeFromCache(QList<FolderNode *> staleFolders)
{
    foreach (FolderNode *fn, staleFolders) {
        removeFromCache(fn->subFolderNodes());
        m_childNodes.remove(fn);
    }
}

void BasicModel::changedSortKey(FolderNode *folder, Node *node)
{
    if (!m_childNodes.contains(folder))
        return; // The directory was not yet mapped, so there is no need to sort it.

    QList<Node *> nodes = m_childNodes.value(folder);
    int oldIndex = nodes.indexOf(node);

    nodes.removeAt(oldIndex);
    QList<Node *>::iterator newPosIt = qLowerBound(nodes.begin(), nodes.end(), node, sortNodes);
    int newIndex = newPosIt - nodes.begin();

    if (newIndex == oldIndex)
        return;

    nodes.insert(newPosIt, node);

    QModelIndex parentIndex = indexForNode(folder);
    if (newIndex > oldIndex)
        ++newIndex; // see QAbstractItemModel::beginMoveRows
    beginMoveRows(parentIndex, oldIndex, oldIndex, parentIndex, newIndex);
    m_childNodes[folder] = nodes;
    endMoveRows();
}

/*!
 * \brief BasicModel::fetchMore
 * \param folder
 */
void BasicModel::fetchMore(FolderNode *folder) const
{
    if (!folder)
        return;

    Q_ASSERT(folder);
    Q_ASSERT(!m_childNodes.contains(folder));

    QList<Node *> nodeList = childNodes(folder);
    m_childNodes.insert(folder, nodeList);
}

void BasicModel::aboutToExpand(FolderNode *folder) const
{
    m_basictree->aboutToFetchMore(folder);
}

/*!
 * \brief BasicModel::childNodes
 * \param parentNode
 * \param blackList
 * \return
 */
QList<Node *> BasicModel::childNodes(FolderNode *parentNode, const QSet<Node *> &blackList) const
{
    QList<Node *> nodeList;

    recursiveAddFolderNodes(parentNode, &nodeList, blackList);
    recursiveAddNodes(parentNode, &nodeList, blackList + nodeList.toSet());

    Utils::sort(nodeList, sortNodes);

    return nodeList;
}

/*!
 * \brief BasicModel::recursiveAddFolderNodes
 * \param startNode
 * \param list
 * \param blackList
 */
void BasicModel::recursiveAddFolderNodes(FolderNode *startNode, QList<Node *> *list, const QSet<Node *> &blackList) const
{
    foreach (FolderNode *folder, startNode->subFolderNodes()) {
        if (folder && !blackList.contains(folder)) {
            recursiveAddFolderNodesImpl(folder, list, blackList);
        }
    }
}

/*!
 * \brief BasicModel::recursiveAddFolderNodesImpl
 * \param startNode
 * \param list
 * \param blackList
 */
void BasicModel::recursiveAddFolderNodesImpl(FolderNode *startNode, QList<Node *> *list, const QSet<Node *> &blackList) const
{
    if (!filter(startNode)) {
        if (!blackList.contains(startNode))
            list->append(startNode);
    } else {
        foreach (FolderNode *folder, startNode->subFolderNodes()) {
            if (folder && !blackList.contains(folder))
                recursiveAddFolderNodesImpl(folder, list, blackList);
        }
    }
}

void BasicModel::recursiveAddNodes(FolderNode *startNode, QList<Node *> *list, const QSet<Node *> &blackList) const
{
    // TODO
    foreach (FolderNode *subFolderNode, startNode->subFolderNodes()) {
        if (!blackList.contains(subFolderNode))
            recursiveAddNodes(subFolderNode, list, blackList);
    }
    foreach (Node *node, startNode->childNodes()) {
        if (!blackList.contains(node) && !filter(node))
            list->append(node);
    }
}

/*!
 * \brief BasicModel::visibleFolderNode
 * \param node
 * \return
 */
FolderNode *BasicModel::visibleFolderNode(FolderNode *node) const
{
    if (!node)
        return 0;

    for (FolderNode *folder = node; folder; folder = folder->parentFolderNode()) {
        if (!filter(folder))
            return folder;
    }

    return 0;
}

/*!
 * \brief BasicModel::filter
 * \param node
 * \return 是否过滤该节点
 */
bool BasicModel::filter(Node *node) const
{
#if 0
    return (m_basictree? !m_basictree->showInTree(node): false);
#else
    // TODO: does not support dynamically change
    Q_UNUSED(node);
    return false;
#endif
}

Node *BasicModel::nodeForId(Core::Id id, FolderNode *folder) const
{
    if (folder->id() == id)
        return folder;

    foreach (auto f, folder->subFolderNodes()) {
        Node *n = nodeForId(id, f);
        if (n)
            return n;
    }

    foreach (auto n, folder->childNodes()) {
        if (n->id() == id)
            return n;
    }

    return 0;
}

void BasicModel::nodeUpdated(Node *node)
{
    QModelIndex idx = indexForNode(node);
    emit dataChanged(idx, idx);
}

void BasicModel::nodeUpdated(Node *node, const QVector<int> &roles)
{
    QModelIndex idx = indexForNode(node);
    emit dataChanged(idx, idx, roles);
}

void BasicModel::folderAboutToBeAdded(FolderNode *parentFolder, FolderNode *newFolder)
{
    Q_UNUSED(newFolder);
    m_parentNodeForChange = parentFolder;
}

void BasicModel::folderAdded()
{
    added(m_parentNodeForChange);
}

void BasicModel::folderAboutToBeRemoved(FolderNode *parentFolder, FolderNode *staleFolder)
{
    foldersAboutToBeRemoved(parentFolder, QList<FolderNode *>() << staleFolder);
}

void BasicModel::folderRemoved()
{
    // nothing to do
}

void BasicModel::nodeAboutToBeAdded(FolderNode *folder, Node *newNode)
{
    Q_UNUSED(newNode);

    m_parentNodeForChange = folder;
}

void BasicModel::nodeAdded()
{
    added(m_parentNodeForChange);
}

void BasicModel::nodeAboutToBeRemoved(FolderNode *folder, Node *staleNode)
{
    // TODO:
    FolderNode *folderNode = visibleFolderNode(folder);
    QList<Node *> newNodeList = childNodes(folderNode, QSet<Node *>() << staleNode);
    removed(folder, newNodeList);
}

void BasicModel::nodeRemoved()
{
    // nothing to do
}

void BasicModel::foldersAboutToBeAdded(FolderNode *parentFolder, const QList<FolderNode *> &newFolders)
{
    Q_UNUSED(newFolders);
    m_parentNodeForChange = parentFolder;
}

void BasicModel::foldersAdded()
{
    added(m_parentNodeForChange);
}

void BasicModel::foldersAboutToBeRemoved(FolderNode *parentFolder, const QList<FolderNode *> &staleFolders)
{
    QSet<Node *> blackList;
    foreach (FolderNode *node, staleFolders) {
        blackList.insert(node);
    }

    FolderNode *folderNode = visibleFolderNode(parentFolder);
    QList<Node *> newNodeList = childNodes(folderNode, blackList);

    removed(folderNode, newNodeList);
    removeFromCache(staleFolders);
}

void BasicModel::foldersRemoved()
{
    // nothing to do
}

void BasicModel::nodesAboutToBeAdded(FolderNode *folder, const QList<Node *> &newNodes)
{

    Q_UNUSED(newNodes);
    m_parentNodeForChange = folder;
}

void BasicModel::nodesAdded()
{
    added(m_parentNodeForChange);
}

void BasicModel::nodesAboutToBeRemoved(FolderNode *folder, const QList<Node *> &staleNodes)
{
    QSet<Node *> blackList;
    foreach (auto node, staleNodes) {
        blackList.insert(node);
    }

    FolderNode *folderNode = visibleFolderNode(folder);
    QList<Node *> newNodeList = childNodes(folderNode, blackList);

    removed(folderNode, newNodeList);
}

void BasicModel::nodesRemoved()
{
    // nothing to do
}

void BasicModel::nodeSortKeyAboutToChange(Node *node)
{
    m_nodeForSortKeyChange = node;
}

void BasicModel::nodeSortKeyChanged()
{
    FolderNode *folderNode = visibleFolderNode(m_nodeForSortKeyChange->parentFolderNode());
    changedSortKey(folderNode, m_nodeForSortKeyChange);
}

void BasicModel::nodeHiddenStateAboutToChange(Node *node)
{
    m_nodeForHiddenStateChange = node;
}

void BasicModel::nodeHiddenStateChanged()
{
    if (m_basictree && m_nodeForHiddenStateChange && m_nodeForHiddenStateChange->isValid()) {
        QModelIndex idx = indexForNode(m_nodeForHiddenStateChange);
        if (idx.isValid()) {
            emit rowHiddenStateChanged(idx.row(), idx.parent(), !m_basictree->showInTree(m_nodeForHiddenStateChange));
        }
    }

    m_nodeForHiddenStateChange = 0;
}

} // namespace ProjectExplorer
