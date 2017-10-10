#include <projectexplorer/basicmodel.h>
#include <projectexplorer/basictreeview.h>

#include "constants.h"
#include "treewidget.h"

namespace MediaSource {
namespace Internal {

TreeWidget::TreeWidget(QWidget *parent)
    : BasicTreeWidget(parent)
    , m_model(0), m_view(0), m_tree(0)
{
    m_model = new PE::BasicModel(this);
    m_view = new PE::BasicTreeView(Constants::C_MEDIASOURCE_DEVICETREEVIEW, this);

    afterInitialized();
}

TreeWidget::~TreeWidget()
{
}

/*!
 * \brief TreeWidget::setBasicTree
 * \param basictree
 *  the ownership of \p basictree is not transfered to this widget
 */
void TreeWidget::setBasicTree(PE::BasicTree *basictree)
{
    if (basictree == m_tree)
        return;

    if (m_tree) {
        // do something
        // clear
    }

    m_tree = basictree;

    startup();
}

ProjectExplorer::BasicModel *TreeWidget::basicModel()
{
    return m_model;
}

ProjectExplorer::BasicTreeView *TreeWidget::treeView()
{
    return m_view;
}

ProjectExplorer::BasicTree *TreeWidget::basicTree()
{
    return m_tree;
}

} // namespace Internal
} // namespace MediaSource
