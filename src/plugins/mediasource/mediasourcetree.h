#ifndef MEDIA_SOURCE_TREE_H
#define MEDIA_SOURCE_TREE_H

#include <projectexplorer/basictree.h>

namespace ProjectExplorer {
class MediaSource;
class Project;
}

namespace MediaSource {
namespace Internal {
class MediaSourceTreePrivate;
class MediaSourceTreeWidget;
}

class PROJECTEXPLORER_EXPORT MediaSourceTree : public ProjectExplorer::BasicTree
{
    Q_OBJECT

public:
    explicit MediaSourceTree(QObject *parent = 0);
    ~MediaSourceTree();

    static MediaSourceTree *instance();

public:
    ProjectExplorer::FolderNode *rootNode() Q_DECL_OVERRIDE;
    ProjectExplorer::Node *currentNode() Q_DECL_OVERRIDE;

    void nodeChanged(ProjectExplorer::BasicTreeWidget *widget) Q_DECL_OVERRIDE;
    void showContextMenu(ProjectExplorer::BasicTreeWidget *focus, const QPoint &globalPos, ProjectExplorer::Node *node) Q_DECL_OVERRIDE;

public:
    bool selectableInTree(ProjectExplorer::Node *node) const Q_DECL_OVERRIDE;

public:
    static void registerWidget(Internal::MediaSourceTreeWidget *widget);
    static void unregisterWidget(Internal::MediaSourceTreeWidget *widget);

public:
    static void updateMediaSourceClasses();

public:
    static void emitBrowseItem(ProjectExplorer::Node *node);

signals:
    void currentNodeChanged(ProjectExplorer::Node *node, ProjectExplorer::Project *project);
    void currentProjectChanged(ProjectExplorer::Project *project);

    // may be 0
    void currentMediaSourceChanged(ProjectExplorer::MediaSource *mediasource);
    void browseMediaSource(ProjectExplorer::MediaSource *mediasource);

protected:
    void timerEvent(QTimerEvent *);

private:
    ProjectExplorer::MediaSource *currentMediaSource();

private:
    static bool hasFocus(Internal::MediaSourceTreeWidget *widget);
    void updateFromMediaSourceTreeWidget(Internal::MediaSourceTreeWidget *widget);
    void updateFromNode(ProjectExplorer::Node *node);

private:
    void startupProjectChanged(ProjectExplorer::Project *currentStartupProject);
    void update(ProjectExplorer::Node *node, ProjectExplorer::Project *currentProject);

private:
    void mediaSourceAboutToBeAdded(ProjectExplorer::MediaSource *source);
    void mediaSourceAdded();

    void mediaSourceAboutToBeRemoved(ProjectExplorer::MediaSource *source);
    void mediaSourceRemoved();

private:
    void mediaSourceAddedImpl(ProjectExplorer::MediaSource *source);
    void updateMediaSourceNodes();

private:
    static void createMediaSource();

private slots:

private:
    Internal::MediaSourceTreePrivate *d;
};

} // namespace ProjectExplorer

#endif // MEDIA_SOURCE_TREE_H
