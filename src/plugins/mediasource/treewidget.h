#ifndef MEDIASOURCE_TREE_WIDGET_H
#define MEDIASOURCE_TREE_WIDGET_H

#include <projectexplorer/basictreewidget.h>

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class TreeWidget : public ProjectExplorer::BasicTreeWidget
{
    Q_OBJECT

public:
    TreeWidget(QWidget *parent = 0);
    ~TreeWidget();

public:
    void setBasicTree(PE::BasicTree *basictree);

private:
    PE::BasicModel *basicModel() Q_DECL_OVERRIDE;
    PE::BasicTreeView *treeView() Q_DECL_OVERRIDE;
    PE::BasicTree *basicTree() Q_DECL_OVERRIDE;

private:
    PE::BasicModel *m_model;
    PE::BasicTreeView *m_view;
    PE::BasicTree *m_tree;
};

} // namespace Internal
} // namespace MediaSource

#endif // MEDIASOURCE_TREE_WIDGET_H
