#include <core/icontext.h>
#include <core/icore.h>

#include "basicmodel.h"
#include "basictreeview.h"

namespace ProjectExplorer {

BasicTreeItemDelegate::BasicTreeItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void BasicTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    if (!index.data(BasicModel::EnabledRole).toBool())
        opt.state &= ~QStyle::State_Enabled;

    QStyledItemDelegate::paint(painter, opt, index);
}


namespace Internal {
class BasicTreeViewPrivate
{
public:
    Core::IContext *m_context;
    mutable int m_cachedSize;
};
} // namespace Internal

BasicTreeView::BasicTreeView(const char *context, QWidget *parent)
    : Utils::NavigationTreeView(parent)
    , d(new Internal::BasicTreeViewPrivate())
{
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);

    d->m_context = new Core::IContext(this);
    d->m_context->setContext(Core::Context(context));
    d->m_context->setWidget(this);
    d->m_cachedSize = -1;

    Core::ICore::addContextObject(d->m_context);

    connect(this, &BasicTreeView::expanded,
            this, &BasicTreeView::invalidateSize);
    connect(this, &BasicTreeView::collapsed,
            this, &BasicTreeView::invalidateSize);
}

BasicTreeView::~BasicTreeView()
{
    Core::ICore::removeContextObject(d->m_context);
    delete d->m_context;
    delete d;
}

void BasicTreeView::setModel(QAbstractItemModel *newModel)
{
    // Note: Don't connect to column signals, as we have only one column

    if (model() == newModel)
        return;

    if (model()) {
        QAbstractItemModel *m = model();
        disconnect(m, &QAbstractItemModel::dataChanged,
                this, &BasicTreeView::invalidateSize);
        disconnect(m, &QAbstractItemModel::layoutChanged,
                this, &BasicTreeView::invalidateSize);
        disconnect(m, &QAbstractItemModel::modelReset,
                this, &BasicTreeView::invalidateSize);
        disconnect(m, &QAbstractItemModel::rowsInserted,
                this, &BasicTreeView::invalidateSize);
        disconnect(m, &QAbstractItemModel::rowsMoved,
                this, &BasicTreeView::invalidateSize);
        disconnect(m, &QAbstractItemModel::rowsRemoved,
                this, &BasicTreeView::invalidateSize);
    }

    if (newModel) {
        connect(newModel, &QAbstractItemModel::dataChanged,
                this, &BasicTreeView::invalidateSize);
        connect(newModel, &QAbstractItemModel::layoutChanged,
                this, &BasicTreeView::invalidateSize);
        connect(newModel, &QAbstractItemModel::modelReset,
                this, &BasicTreeView::invalidateSize);
        connect(newModel, &QAbstractItemModel::rowsInserted,
                this, &BasicTreeView::invalidateSize);
        connect(newModel, &QAbstractItemModel::rowsMoved,
                this, &BasicTreeView::invalidateSize);
        connect(newModel, &QAbstractItemModel::rowsRemoved,
                this, &BasicTreeView::invalidateSize);
    }

    Utils::NavigationTreeView::setModel(newModel);
}

void BasicTreeView::invalidateSize()
{
    d->m_cachedSize = -1;
}

int BasicTreeView::sizeHintForColumn(int column) const
{
    if (d->m_cachedSize < 0)
        d->m_cachedSize = Utils::NavigationTreeView::sizeHintForColumn(column);

    return d->m_cachedSize;
}

QModelIndexList BasicTreeView::selectedIndexes() const
{
    return NavigationTreeView::selectedIndexes();
}

void BasicTreeView::selectIndexes(const QModelIndexList &indexes)
{
    auto m = selectionModel();
    if (m) {
        auto mode = selectionMode();
        if (mode != NoSelection) {
            if (mode != SingleSelection || indexes.size() == 1) {
                setSelectionMode(MultiSelection);
                foreach (auto idx, indexes) {
                    m->select(idx, QItemSelectionModel::Select|QItemSelectionModel::Rows);
                }
                setSelectionMode(mode);
            }
        }
    }
}

} // namespace ProjectExplorer
