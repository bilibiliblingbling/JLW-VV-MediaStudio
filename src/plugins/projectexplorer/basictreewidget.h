#ifndef BASIC_TREE_WIDGET_H
#define BASIC_TREE_WIDGET_H

#include <QWidget>

#include "projectexplorer_global.h"

namespace ProjectExplorer {

class BasicModel;
class BasicTree;
class BasicTreeView;
class Node;
class SelectTreeDialog;
class PROJECTEXPLORER_EXPORT BasicTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasicTreeWidget(QWidget *parent = 0);
    virtual ~BasicTreeWidget();

public:
    Node *currentNode();

protected:
    void afterInitialized();

    void startup();
    void shutdown();

private:
    virtual BasicModel *basicModel() = 0;
    virtual BasicTreeView *treeView() = 0;

    virtual BasicTree *basicTree() = 0;

private:
    virtual void showContextMenu(const QPoint &pos);

private:
    void initView();
    void setCurrentItem(Node *node);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    friend class SelectTreeDialog;
};

} // namespace ProjectExplorer

#endif // BASIC_TREE_WIDGET_H
