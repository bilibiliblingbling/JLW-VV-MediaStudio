#ifndef CLEARVISION_CAMERA_TREE_H
#define CLEARVISION_CAMERA_TREE_H

#include <QHash>

#include <core/id.h>
#include <mediasource/cameratree.h>

namespace ClearVision {
namespace Internal {

namespace MS = MediaSource;
namespace PE = ProjectExplorer;

class MediaSource;
class ObjectNode;
class UObject;
class CameraTree : public MS::CameraTree
{
    Q_OBJECT

public:
    CameraTree(QObject *parent = 0);
    ~CameraTree();

    static CameraTree *instance();

public:
    void setCurrentMediaSource(PE::MediaSource *mediasource);

public:
    PE::FolderNode *rootNode() Q_DECL_OVERRIDE;
    PE::Node *currentNode() Q_DECL_OVERRIDE;

    void nodeChanged(PE::BasicTreeWidget *widget) Q_DECL_OVERRIDE;

public:
    bool canFetchMoreInTree(PE::FolderNode *folder) const Q_DECL_OVERRIDE;
    void aboutToFetchMore(PE::FolderNode *folder) Q_DECL_OVERRIDE;
    bool selectableInTree(PE::Node *node) const Q_DECL_OVERRIDE;
    bool showInTree(PE::Node *node) const Q_DECL_OVERRIDE;

private:
    void update(PE::Node *node, MediaSource *currentMediaSource);

private:
    void objectAboutToBeAdded(UObject *obj);
    void objectAdded();

    void objectAboutToBeRemoved(UObject *obj);
    void objectRemoved();

    void objectsAboutToBeAdded(Core::Id unitId, const QList<UObject *> &objects);
    void objectsAdded();

    void objectsAboutToBeCleared(UObject *obj);
    void objectsCleared();

    void fetchingMore(Core::Id unitId);
    void fetchMoreFailed(Core::Id unitId, const QString &error);
    void fetchMoreSucceed(Core::Id unitId);

private:
    void objectAddedImpl(UObject *obj);
    void objectsAddedImpl(PE::FolderNode *folder, const QList<UObject *> &objects);

    PE::FolderNode *folderOfId(PE::FolderNode *startFolder, Core::Id id);

private:
    PE::VirtualFolderNode *m_virtualRootNode;
    PE::FolderNode *m_rootNode;

    QHash<Core::Id, ObjectNode *> m_folderNodes;

private:
    PE::Node *m_currentNode;
    MediaSource *m_currentMediaSource;

private:
    PE::FolderNode *m_parentNodeForChange;
    UObject *m_objectAdded;
};

} // namespace Internal
} // namespace ClearVision


#endif // CLEARVISION_CAMERA_TREE_H
