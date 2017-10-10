#ifndef PROJECT_TREE_H
#define PROJECT_TREE_H

#include <core/icontext.h>

#include "projectexplorer_global.h"


namespace ProjectExplorer {
class Node;
class FolderNode;
class Project;

namespace Internal { class ProjectTreeWidget; }

class PROJECTEXPLORER_EXPORT ProjectTree : public QObject
{
    Q_OBJECT

public:
    explicit ProjectTree(QObject *parent = 0);
    ~ProjectTree();

    static ProjectTree *instance();

public:
    static void registerWidget(Internal::ProjectTreeWidget *widget);
    static void unregisterWidget(Internal::ProjectTreeWidget *widget);
    static void nodeChanged(Internal::ProjectTreeWidget *widget);
    static Project *projectForNode(Node *node);

public:
    static void aboutToShutDown();
    static void showContextMenu(Internal::ProjectTreeWidget *focus, const QPoint &globalPos, Node *node);

public:
    // common
    void emitNodeAboutToUpdated(Node *node);

    void emitNodeSortKeyAboutToChange(Node *node);
    void emitNodeSortKeyChanged(Node *node);

    void collapseAll();

    // projects
    void emitAboutToChangeShowInSimpleTree(FolderNode *node);
    void emitShowInSimpleTreeChanged(FolderNode *node);

    // folders
    void emitFoldersAboutToBeAdded(FolderNode *parentFolder,
                                   const QList<FolderNode*> &newFolders);
    void emitFoldersAdded(FolderNode *folder);

    void emitFoldersAboutToBeRemoved(FolderNode *parentFolder,
                                     const QList<FolderNode*> &staleFolders);
    void emitFoldersRemoved(FolderNode *folder);

    // others
    void emitNodesAboutToBeAdded(FolderNode *folder,
                                 const QList<Node *> &nodes);
    void emitNodesAdded(FolderNode *folder);

    void emitNodesAboutTobeRemoved(FolderNode *folder,
                                   const QList<Node *> &staleNodes);
    void emitNodesRemoved(FolderNode *folder);

signals:
    // source, query source records in browse pane
    // seg, open seg in browse pane
    // sequence, open sequence in timeline pane
    // drag and drop
    void currentProjectChanged(Project *project);

    void foldersAboutToBeAdded(FolderNode *parentFolder, const QList<FolderNode *> &newFolders);
    void foldersAdded();

    void foldersAboutToBeRemoved(FolderNode *parentFolder, const QList<FolderNode *> &staleFolders);
    void foldersRemoved();

    // Media Sources
    void mediaSourcesAboutToBeAdded(FolderNode *parentFolder, const QList<Node *> &newMediaSources);
    void mediaSourcesAdded();

    void mediaSourcesAboutToBeRemoved();
    void mediaSourcesRemoved();

    // Segments
    void segmentsAboutToBeAdded();
    void segmentsAdded();

    void segmentsAboutToBeRemoved();
    void segmentsAboutRemoved();

    // Sequences
    void sequencesAboutToBeAdded();
    void sequencesAdded();

    void sequencesAboutToBeRemoved();
    void sequencesRemoved();

    // Destinations

private:
};

} // namespace ProjectExplorer

#endif // PROJECT_TREE_H
