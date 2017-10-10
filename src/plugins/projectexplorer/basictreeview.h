#ifndef BASIC_TREE_VIEW_H
#define BASIC_TREE_VIEW_H

#include <QStyledItemDelegate>

#include <utils/navigationtreeview.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {

class PROJECTEXPLORER_EXPORT BasicTreeItemDelegate : public QStyledItemDelegate
{
public:
    BasicTreeItemDelegate(QObject *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


class SelectTreeDialog;
namespace Internal { class BasicTreeViewPrivate; }
class PROJECTEXPLORER_EXPORT BasicTreeView : public Utils::NavigationTreeView
{
public:
    BasicTreeView(const char *context, QWidget *parent = 0);
    ~BasicTreeView();

public:
    void setModel(QAbstractItemModel *newModel);
    int sizeHintForColumn(int column) const;

public:
    void invalidateSize();

protected:
    QModelIndexList selectedIndexes() const;
    void selectIndexes(const QModelIndexList &indexes);

private:
    Internal::BasicTreeViewPrivate *d;

private:
    friend class SelectTreeDialog;
};

} // namespace ProjectExplorer

#endif // BASIC_TREE_VIEW_H
