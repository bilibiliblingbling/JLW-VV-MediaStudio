#ifndef BASIC_TREE_H
#define BASIC_TREE_H

#include <QObject>

#include "projectexplorer_global.h"

namespace ProjectExplorer {

class BasicTreeWidget;
class Node;
class FolderNode;
class VirtualFolderNode;

namespace Internal { class BasicTreeManager; }

class PROJECTEXPLORER_EXPORT BasicTree : public QObject
{
    Q_OBJECT

public:
    BasicTree(QObject *parent = 0);
    virtual ~BasicTree();

public:
    virtual FolderNode *rootNode() = 0;
    virtual Node *currentNode() = 0;

    virtual void nodeChanged(BasicTreeWidget *widget) = 0;
    virtual void showContextMenu(BasicTreeWidget *focus, const QPoint &globalPos, Node *node);

public:
    virtual bool canFetchMoreInTree(FolderNode *folder) const;
    virtual void aboutToFetchMore(FolderNode *folder);
    virtual bool renamableInTree(Node *node) const;
    virtual bool selectableInTree(Node *node) const;
    virtual bool dragableInTree(Node *node) const;
    virtual bool showInTree(Node *node) const;

signals:
    void nodeUpdated(Node *node);
    void nodeUpdated(Node *node, const QVector<int> &roles);

    void folderAboutToBeAdded(FolderNode *parentFolder, FolderNode *newFolder);
    void folderAdded();

    void foldersAboutToBeAdded(FolderNode *parentFolder, const QList<FolderNode *> &newFolders);
    void foldersAdded();

    void folderAboutToBeRemoved(FolderNode *parentFolder, FolderNode *staleFolder);
    void folderRemoved();

    void foldersAboutToBeRemoved(FolderNode *parentFolder, const QList<FolderNode *> &staleFolders);
    void foldersRemoved();

    void nodeAboutToBeAdded(FolderNode *folder, Node *newNode);
    void nodeAdded();

    void nodesAboutToBeAdded(FolderNode *folder, const QList<Node *> newNodes);
    void nodesAdded();

    void nodeAboutToBeRemoved(FolderNode *folder, Node *staleNode);
    void nodeRemoved();

    void nodesAboutToBeRemoved(FolderNode *folder, const QList<Node *> staleNodes);
    void nodesRemoved();

    void nodeSortKeyAboutToChange(Node *node);
    void nodeSortKeyChanged();

    void nodeHiddenStateAboutToChange(Node *node);
    void nodeHiddenStateChanged();

    void aboutToShowContextMenu(Node *node);

protected:
    bool isInNodeHierarchy(Node *node);

private:
    friend class Internal::BasicTreeManager;
};

} // namespace ProjectExplorer


#endif // BASIC_TREE_H
