#ifndef SELECT_TREE_DIALOG_H
#define SELECT_TREE_DIALOG_H

#include <QDialog>

#include <core/id.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {

class Node;
class BasicTreeWidget;
namespace Internal { class SelectTreeDialogPrivate; }
class PROJECTEXPLORER_EXPORT SelectTreeDialog : public QDialog
{
    Q_OBJECT

public:
    SelectTreeDialog(QWidget *parent = 0);
    virtual ~SelectTreeDialog();

public:
    void setBasicTreeWidget(BasicTreeWidget *widget);

public:
    bool appendResulsts() const;
    QList<Node *> selectedNodes();
    void selectNodes(const QList<Node *> &nodes);
    void selectNodes(const QList<Core::Id> &nodes);

    void clearSelection();

signals:
    void hidingSelectTree(const QList<ProjectExplorer::Node *> &nodes, bool append);

protected:
    void focusOutEvent(QFocusEvent *e);

private slots:
    void okButtonClicked();

private:
    Internal::SelectTreeDialogPrivate *d;
};

} // namespace ProjectExplorer

#endif // SELECT_TREE_DIALOG_H
