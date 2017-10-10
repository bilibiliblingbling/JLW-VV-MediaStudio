#ifndef BASIC_TREE_MANAGER_H
#define BASIC_TREE_MANAGER_H

#include <QObject>

namespace ProjectExplorer {
class BasicTree;
class Node;
class FolderNode;
namespace Internal {

// can be internal
class BasicTreeManager : public QObject
{
    Q_OBJECT

public:
    BasicTreeManager(QObject *parent = 0);
    ~BasicTreeManager();

    static BasicTreeManager *instance();

public:
    static void registerBasicTree(BasicTree *basictree);
    static void unregisterBasicTree(BasicTree *basictree);

public:
    static void emitNodeUpdated(Node *node);

    static void emitFolderAboutToBeAdded(FolderNode *parentFolder, FolderNode  *newFolder);
    static void emitFolderAdded(FolderNode *parentFolder);

    static void emitFoldersAboutToBeAdded(FolderNode *parentFolder, const QList<FolderNode *> &newFolders);
    static void emitFoldersAdded(FolderNode *parentFolder);

    static void emitFolderAboutToBeRemoved(FolderNode *parentFolder, FolderNode *staleFolder);
    static void emitFolderRemoved(FolderNode *parentFolder);

    static void emitFoldersAboutToBeRemoved(FolderNode *parentFolder, const QList<FolderNode *> &staleFolders);
    static void emitFoldersRemoved(FolderNode *parentFolder);

    static void emitNodeAboutToBeAdded(FolderNode *folder, Node *newNode);
    static void emitNodeAdded(FolderNode *folder);

    static void emitNodesAboutToBeAdded(FolderNode *folder, const QList<Node *> &newNodes);
    static void emitNodesAdded(FolderNode *folder);

    static void emitNodeAboutToBeRemoved(FolderNode *folder, Node *staleNode);
    static void emitNodeRemoved(FolderNode *folder);

    static void emitNodesAboutTobeRemoved(FolderNode *folder, const QList<Node *> &staleNodes);
    static void emitNodesRemoved(FolderNode *folder);

    static void emitNodeSortKeyAboutToChange(Node *node);
    static void emitNodeSortKeyChanged(Node *node);

    static void emitNodeHiddenStateAboutToChange(Node *node);
    static void emitNodeHiddenStateChanged(Node *node);

private:
    static void traversal(Node *node, std::function<void (BasicTree *)> func);

private:
    QList<BasicTree *> m_basictrees;
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // BASIC_TREE_MANAGER_H
