#include <QBoxLayout>

#include <utils/navigationtreeview.h>

#include "basicmodel.h"
#include "basictree.h"
#include "basictreeview.h"
#include "basictreewidget.h"

namespace ProjectExplorer {

BasicTreeWidget::BasicTreeWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

BasicTreeWidget::~BasicTreeWidget()
{
}

Node *BasicTreeWidget::currentNode()
{
    return basicModel()->nodeForIndex(treeView()->currentIndex());
}

void BasicTreeWidget::afterInitialized()
{
    // the treeView() and basicModel() cannot be changed
    treeView()->setItemDelegate(new BasicTreeItemDelegate(this));
    treeView()->setModel(basicModel());
    layout()->addWidget(treeView());
    setFocusProxy(treeView());
    treeView()->installEventFilter(this);

    connect(basicModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
            treeView(), SLOT(expand(QModelIndex)));
    connect(basicModel(), &BasicModel::rowHiddenStateChanged,
            treeView(), &BasicTreeView::setRowHidden);

    connect(treeView(), SIGNAL(expanded(QModelIndex)),
            basicModel(), SLOT(aboutToExpand(QModelIndex)));

    connect(treeView()->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &BasicTreeWidget::currentChanged);
    connect(treeView(), &QTreeView::customContextMenuRequested,
            this, &BasicTreeWidget::showContextMenu);
}

void BasicTreeWidget::startup()
{
    BasicTree *bt = basicTree();
    basicModel()->setBasicTree(bt);
    basicModel()->setRootNode(bt? bt->rootNode(): 0);

    initView();
}

void BasicTreeWidget::shutdown()
{
}

void BasicTreeWidget::showContextMenu(const QPoint &pos)
{
    QModelIndex idx = treeView()->indexAt(pos);
    Node *node = basicModel()->nodeForIndex(idx);
    basicTree()->showContextMenu(this, treeView()->mapToGlobal(pos), node);
}

void BasicTreeWidget::initView()
{
    QModelIndex rootIndex = basicModel()->index(0, 0);
    treeView()->setRootIndex(rootIndex);

    while (basicModel()->canFetchMore(rootIndex))
        basicModel()->fetchMore(rootIndex);

    for (int i = 0; i < basicModel()->rowCount(rootIndex); ++i)
        treeView()->expand(basicModel()->index(i, 0, rootIndex));

    if (BasicTree *bt = basicTree())
        setCurrentItem(bt->currentNode());
}

void BasicTreeWidget::setCurrentItem(Node *node)
{
    const QModelIndex mainIndex = basicModel()->indexForNode(node);

    if (mainIndex.isValid()) {
        if (mainIndex != treeView()->selectionModel()->currentIndex()) {
            treeView()->setCurrentIndex(mainIndex);
            treeView()->scrollTo(mainIndex);
        }
    } else {
        treeView()->clearSelection();
    }
}

void BasicTreeWidget::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);

    basicTree()->nodeChanged(this);
}

} // namespace ProjectExplorer
