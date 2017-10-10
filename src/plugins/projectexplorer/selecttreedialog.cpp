#include <QCheckBox>
#include <QPushButton>
#include <QBoxLayout>

#include <utils/tmplog.h>

#include "constants.h"
#include "selecttreedialog.h"
#include "selecttreedialog_p.h"

namespace ProjectExplorer {

SelectTreeDialog::SelectTreeDialog(QWidget *parent)
    : QDialog(parent, Qt::Popup)
    , d(new Internal::SelectTreeDialogPrivate())
{
    d->m_widget = 0;
    d->m_appendResults = new QCheckBox(tr("&Append"), this);
    d->m_okButton = new QPushButton(tr("&Ok"), this);
    connect(d->m_okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

    d->m_appendResults->setFocusProxy(this);
    d->m_okButton->setFocusProxy(this);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    layout->addWidget(d->m_appendResults, Qt::AlignRight);
    layout->addWidget(d->m_okButton, Qt::AlignRight);
    layout->setStretch(0, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2, 4, 2, 2);
    mainLayout->addLayout(layout);
    setLayout(mainLayout);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    setFixedWidth(240);
}

SelectTreeDialog::~SelectTreeDialog()
{
    delete d;
}

/*!
 * \brief SelectTreeDialog::setBasicTreeWidget
 * \param widget
 *  The ownership of \p widget will be transferred to this QWidget.
 */
void SelectTreeDialog::setBasicTreeWidget(ProjectExplorer::BasicTreeWidget *widget)
{
    if (d->m_widget) {
        TM_DBG << "widget has already existed, and cannot change by now";
        return;
    }
    d->m_widget = widget;
    d->m_widget->setParent(this);
    d->m_widget->setFocusProxy(this);
    d->m_widget->treeView()->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(d->m_widget->treeView(), SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(okButtonClicked()));
    setWindowTitle(tr("Select %1").arg(d->m_widget->windowTitle()));
    QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(layout());
    boxlayout->insertWidget(0, d->m_widget, 1);
}

bool SelectTreeDialog::appendResulsts() const
{
    return d->m_appendResults->isChecked();
}

QList<Node *> SelectTreeDialog::selectedNodes()
{
    QList<Node *> nodes;
    if (d->m_widget) {
        auto indexes = d->m_widget->treeView()->selectedIndexes();
        auto model = d->m_widget->basicModel();
        foreach (auto idx, indexes) {
            Node *n = model->nodeForIndex(idx);
            if (n) {
                nodes.append(n);
            }
        }
    }
    return nodes;
}

void SelectTreeDialog::selectNodes(const QList<Node *> &nodes)
{
    if (d->m_widget) {
        QModelIndexList indexes;
        auto model = d->m_widget->basicModel();
        foreach (auto n, nodes) {
            QModelIndex idx = model->indexForNode(n);
            if (idx.isValid()) {
                indexes.append(idx);
            }
        }
        d->m_widget->treeView()->selectIndexes(indexes);
    }
}

void SelectTreeDialog::selectNodes(const QList<Core::Id> &nodes)
{
    if (d->m_widget) {
        QModelIndexList indexes;
        auto model = d->m_widget->basicModel();
        foreach (auto n, nodes) {
            QModelIndex idx = model->indexForId(n);
            if (idx.isValid()) {
                indexes.append(idx);
            }
        }
        d->m_widget->treeView()->selectIndexes(indexes);
    }
}

void SelectTreeDialog::clearSelection()
{
    if (d->m_widget)
        d->m_widget->treeView()->clearSelection();
}

void SelectTreeDialog::focusOutEvent(QFocusEvent *e)
{
#if 0
    reject();
#else
    QDialog::focusOutEvent(e);
#endif
}

void SelectTreeDialog::okButtonClicked()
{
    emit hidingSelectTree(selectedNodes(), d->m_appendResults->isChecked());
    accept();
}

} // namespace MediaSource
