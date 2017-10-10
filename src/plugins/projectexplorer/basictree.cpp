#include "basicnodes.h"
#include "basictree.h"
#include "basictreemanager.h"
#include "sessionmanager.h"

namespace ProjectExplorer {

BasicTree::BasicTree(QObject *parent)
    : QObject(parent)
{
    Internal::BasicTreeManager::registerBasicTree(this);
}

BasicTree::~BasicTree()
{
    Internal::BasicTreeManager::unregisterBasicTree(this);
}

void BasicTree::showContextMenu(BasicTreeWidget *, const QPoint &, Node *)
{
}

bool BasicTree::canFetchMoreInTree(FolderNode *) const
{
    return true;
}

void BasicTree::aboutToFetchMore(FolderNode *)
{
}

bool BasicTree::renamableInTree(Node *) const
{
    return false;
}

bool BasicTree::selectableInTree(Node *) const
{
    return true;
}

bool BasicTree::dragableInTree(Node *) const
{
    return false;
}

bool BasicTree::showInTree(Node *node) const
{
    return true;
}

bool BasicTree::isInNodeHierarchy(Node *node)
{
    if (!node || !node->isValid())
        return false;

    FolderNode *root = rootNode();
    if (!root || !root->isValid())
        return false;

    if (node == root)
        return true;

    FolderNode *folder = node->parentFolderNode();
    for (; folder && folder->isValid(); folder = folder->parentFolderNode()) {
        if (folder == root)
            return true;
    }

    return false;
}

} // namespace ProjectExplorer
