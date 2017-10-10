#include "projecttree.h"

namespace ProjectExplorer {
namespace Internal {
class ProjectTreePrivate
{
public:
    Project *m_currentProject;
    QList<Internal::ProjectTreeWidget *> m_projectTreeWidgets;

    Internal::ProjectTreeWidget *m_focusProjectTreeWidget;  // ForContextMenu
    Core::Context m_lastProjectContext;
};
} // namespace Internal

/*!
    负责管理当前 project
    对外提供当前 project 相关的变化通知

 */

static ProjectTree *s_instance = 0;

ProjectTree::ProjectTree(QObject *parent)
    : QObject(parent)
{
    s_instance = this;
}

ProjectTree::~ProjectTree()
{
    s_instance = 0;
}

ProjectTree *ProjectTree::instance()
{
    return s_instance;
}

void ProjectTree::emitNodeAboutToUpdated(Node *node)
{

}

void ProjectTree::emitNodeSortKeyAboutToChange(Node *node)
{

}

void ProjectTree::emitNodeSortKeyChanged(Node *node)
{

}

void ProjectTree::collapseAll()
{

}

void ProjectTree::emitAboutToChangeShowInSimpleTree(FolderNode *node)
{

}

void ProjectTree::emitShowInSimpleTreeChanged(FolderNode *node)
{

}

void ProjectTree::emitFoldersAboutToBeAdded(FolderNode *parentFolder, const QList<FolderNode *> &newFolders)
{

}

void ProjectTree::emitFoldersAdded(FolderNode *folder)
{

}

void ProjectTree::emitFoldersAboutToBeRemoved(FolderNode *parentFolder, const QList<FolderNode *> &staleFolders)
{

}

void ProjectTree::emitFoldersRemoved(FolderNode *folder)
{

}

} // namespace ProjectExplorer
