#ifndef SELECT_TREE_WIDGET_P_H
#define SELECT_TREE_WIDGET_P_H

#include <projectexplorer/basicmodel.h>
#include <projectexplorer/basictreeview.h>
#include <projectexplorer/basictreewidget.h>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QPushButton;
QT_END_NAMESPACE


namespace ProjectExplorer {
namespace Internal {

class SelectTreeDialogPrivate
{
public:
    BasicTreeWidget *m_widget;
    QCheckBox *m_appendResults;
    QPushButton *m_okButton;
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // SELECT_TREE_WIDGET_P_H
