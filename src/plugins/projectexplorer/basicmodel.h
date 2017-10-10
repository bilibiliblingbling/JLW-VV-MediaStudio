#ifndef BASIC_MODEL_H
#define BASIC_MODEL_H

#include <QAbstractItemModel>
#include <QSet>

#include <core/id.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {
class BasicTree;

class Node;
class FolderNode;

class PROJECTEXPLORER_EXPORT BasicModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum ModelRoles {
        EnabledRole = Qt::UserRole
    };

public:
    BasicModel(QObject *parent);
    virtual ~BasicModel();

    void setBasicTree(BasicTree *basictree);

    void setRootNode(FolderNode *root);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent) const;

public:
    QMimeData *mimeData(const QModelIndexList &indexes) const;

public:
    Node *nodeForIndex(const QModelIndex &index) const;
    QModelIndex indexForNode(const Node *node) const;

    Node *nodeForId(Core::Id id) const;
    QModelIndex indexForId(Core::Id id) const;

public slots:
    void aboutToExpand(const QModelIndex &parent) const;

signals:
    void rowHiddenStateChanged(int row, const QModelIndex &parent, bool hide);

protected:
    bool isRootNode(Node *node) const;

private:
    void added(FolderNode *folder);
    void added(FolderNode *parentFolder, const QList<Node *> &newNodeList);
    void removed(FolderNode *parentFolder, const QList<Node *> &newNodeList);
    void removeFromCache(QList<FolderNode *> staleFolders);
    void changedSortKey(FolderNode *folder, Node *node);

    void fetchMore(FolderNode *folder) const;
    void aboutToExpand(FolderNode *folder) const;

    QList<Node *> childNodes(FolderNode *parentNode, const QSet<Node *> &blackList = QSet<Node *>()) const;

    void recursiveAddFolderNodes(FolderNode *startNode, QList<Node *> *list, const QSet<Node *> &blackList = QSet<Node *>()) const;
    void recursiveAddFolderNodesImpl(FolderNode *startNode, QList<Node *> *list, const QSet<Node *> &blackList = QSet<Node *>()) const;
    void recursiveAddNodes(FolderNode *startNode, QList<Node *> *list, const QSet<Node *> &blackList = QSet<Node *>()) const;

    FolderNode *visibleFolderNode(FolderNode *node) const;
    bool filter(Node *node) const;

    Node *nodeForId(Core::Id id, FolderNode *folder) const;

private slots:
    void nodeUpdated(Node *node);
    void nodeUpdated(Node *node, const QVector<int> &roles);

    void folderAboutToBeAdded(FolderNode *parentFolder, FolderNode *newFolder);
    void folderAdded();

    void folderAboutToBeRemoved(FolderNode *parentFolder, FolderNode *staleFolder);
    void folderRemoved();

    void nodeAboutToBeAdded(FolderNode *folder, Node *newNode);
    void nodeAdded();

    void nodeAboutToBeRemoved(FolderNode *folder, Node *staleNode);
    void nodeRemoved();

    void foldersAboutToBeAdded(FolderNode *parentFolder, const QList<FolderNode *> &newFolders);
    void foldersAdded();

    void foldersAboutToBeRemoved(FolderNode *parentFolder, const QList<FolderNode *> &staleFolders);
    void foldersRemoved();

    void nodesAboutToBeAdded(FolderNode *folder, const QList<Node *> &newNodes);
    void nodesAdded();

    void nodesAboutToBeRemoved(FolderNode *folder, const QList<Node *> &staleNodes);
    void nodesRemoved();

    void nodeSortKeyAboutToChange(Node *node);
    void nodeSortKeyChanged();

    void nodeHiddenStateAboutToChange(Node *node);
    void nodeHiddenStateChanged();

private:
    BasicTree *m_basictree;

    FolderNode *m_rootNode;
    mutable QHash<FolderNode *, QList<Node *> > m_childNodes;

    FolderNode *m_parentNodeForChange;
    Node *m_nodeForSortKeyChange;
    Node *m_nodeForHiddenStateChange;
};

} // namespace ProjectExplorer

#endif // BASIC_MODEL_H
