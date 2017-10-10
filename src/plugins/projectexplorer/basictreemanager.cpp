#include "basicnodes.h"
#include "basictree.h"
#include "basictreemanager.h"

namespace ProjectExplorer {
namespace Internal {

/*!
 *
 * 各个 BasicTree 注册在 BasicTreeManager 中,
 * 当由节点发生变化时, 由 BasicTreeManager 根据节点所属 BasicTree 发送信号
 *
 */

static BasicTreeManager *s_instance = 0;

BasicTreeManager::BasicTreeManager(QObject *parent)
    : QObject(parent)
{
    setObjectName(QLatin1String("ProjectExplorer.BasicTreeManager"));

    s_instance = this;
}

BasicTreeManager::~BasicTreeManager()
{
    s_instance = 0;
}

BasicTreeManager *BasicTreeManager::instance()
{
    return s_instance;
}

void BasicTreeManager::registerBasicTree(BasicTree *basictree)
{
    s_instance->m_basictrees.append(basictree);
}

void BasicTreeManager::unregisterBasicTree(BasicTree *basictree)
{
    s_instance->m_basictrees.removeOne(basictree);
}

void BasicTreeManager::emitNodeUpdated(Node *node)
{
    traversal(node, [node](BasicTree *basictree) { emit basictree->nodeUpdated(node); });
}

void BasicTreeManager::emitFolderAboutToBeAdded(FolderNode *parentFolder, FolderNode *newFolder)
{
    traversal(parentFolder,
              [parentFolder, newFolder](BasicTree *basictree) { emit basictree->folderAboutToBeAdded(parentFolder, newFolder); });
}

void BasicTreeManager::emitFolderAdded(FolderNode *parentFolder)
{
    traversal(parentFolder, [](BasicTree *basictree) { emit basictree->folderAdded(); });
}

void BasicTreeManager::emitFoldersAboutToBeAdded(FolderNode *parentFolder, const QList<FolderNode *> &newFolders)
{
    traversal(parentFolder,
              [parentFolder, &newFolders](BasicTree *basictree) { emit basictree->foldersAboutToBeAdded(parentFolder, newFolders); });
}

void BasicTreeManager::emitFoldersAdded(FolderNode *parentFolder)
{
    traversal(parentFolder, [](BasicTree *basictree) { emit basictree->foldersAdded(); });
}

void BasicTreeManager::emitFolderAboutToBeRemoved(FolderNode *parentFolder, FolderNode *staleFolder)
{
    traversal(parentFolder,
              [parentFolder, staleFolder](BasicTree *basictree) { emit basictree->folderAboutToBeRemoved(parentFolder, staleFolder); });
}

void BasicTreeManager::emitFolderRemoved(FolderNode *parentFolder)
{
    traversal(parentFolder, [](BasicTree *basictree) { emit basictree->folderRemoved(); });
}

void BasicTreeManager::emitFoldersAboutToBeRemoved(FolderNode *parentFolder, const QList<FolderNode *> &staleFolders)
{
    traversal(parentFolder,
              [parentFolder, &staleFolders](BasicTree *basictree) { emit basictree->foldersAboutToBeRemoved(parentFolder, staleFolders); });
}

void BasicTreeManager::emitFoldersRemoved(FolderNode *parentFolder)
{
    traversal(parentFolder, [](BasicTree *basictree) { emit basictree->foldersRemoved(); });
}

void BasicTreeManager::emitNodeAboutToBeAdded(FolderNode *folder, Node *newNode)
{
    traversal(folder, [folder, newNode](BasicTree *basictree) { emit basictree->nodeAboutToBeAdded(folder, newNode); });
}

void BasicTreeManager::emitNodeAdded(FolderNode *folder)
{
    traversal(folder, [](BasicTree *basictree) { emit basictree->nodeAdded(); });
}

void BasicTreeManager::emitNodesAboutToBeAdded(FolderNode *folder, const QList<Node *> &newNodes)
{
    traversal(folder,
              [folder, &newNodes](BasicTree *basictree) { emit basictree->nodesAboutToBeAdded(folder, newNodes); });
}

void BasicTreeManager::emitNodesAdded(FolderNode *folder)
{
    traversal(folder, [folder](BasicTree *basictree) { emit basictree->nodesAdded(); });
}

void BasicTreeManager::emitNodeAboutToBeRemoved(FolderNode *folder, Node *staleNode)
{
    traversal(folder,
              [folder, staleNode](BasicTree *basictree) { emit basictree->nodeAboutToBeRemoved(folder, staleNode); });
}

void BasicTreeManager::emitNodeRemoved(FolderNode *folder)
{
    traversal(folder, [folder](BasicTree *basictree) { emit basictree->nodeRemoved(); });
}

void BasicTreeManager::emitNodesAboutTobeRemoved(FolderNode *folder, const QList<Node *> &staleNodes)
{
    traversal(folder,
              [folder, &staleNodes](BasicTree *basictree) { emit basictree->nodesAboutToBeRemoved(folder, staleNodes); });
}

void BasicTreeManager::emitNodesRemoved(FolderNode *folder)
{
    traversal(folder, [folder](BasicTree *basictree) { emit basictree->nodesRemoved(); });
}

void BasicTreeManager::emitNodeSortKeyAboutToChange(Node *node)
{
    traversal(node, [node](BasicTree *basictree) { emit basictree->nodeSortKeyAboutToChange(node); });
}

void BasicTreeManager::emitNodeSortKeyChanged(Node *node)
{
    traversal(node, [](BasicTree *basictree) { emit basictree->nodeSortKeyChanged(); });
}

void BasicTreeManager::emitNodeHiddenStateAboutToChange(Node *node)
{
    traversal(node, [node](BasicTree *basictree) { emit basictree->nodeHiddenStateAboutToChange(node); });
}

void BasicTreeManager::emitNodeHiddenStateChanged(Node *node)
{
    traversal(node, [](BasicTree *basictree) { emit basictree->nodeHiddenStateChanged(); });
}

void BasicTreeManager::traversal(Node *node, std::function<void (BasicTree *)> func)
{
    foreach (BasicTree *basictree, s_instance->m_basictrees) {
        if (basictree->isInNodeHierarchy(node))
            func(basictree);
    }
}

} // namespace Internal
} // namespace ProjectExplorer
