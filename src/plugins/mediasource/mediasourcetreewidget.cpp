#include <QVBoxLayout>

#include <projectexplorer/basicmodel.h>
#include <projectexplorer/basictreeview.h>

#include <utils/tmplog.h>

#include "constants.h"
#include "mediasourcetree.h"
#include "mediasourcetreewidget.h"


/*!
 *  根据 Project 更新当前显示数据,
 *  MediaSource 中主要是媒体源相关的展示
 *
 *
 *
 */

namespace PE = ProjectExplorer;

namespace MediaSource {
namespace Internal {

MediaSourceTreeWidget::MediaSourceTreeWidget(QWidget *parent)
    : BasicTreeWidget(parent)
    , m_view(0)
    , m_model(0)
{
    // TODO
    m_model = new PE::BasicModel(this);
    m_view = new PE::BasicTreeView(Constants::C_MEDIASOURCE_TREEVIEW, this);

    afterInitialized();
    startup();

    connect(m_view, &QTreeView::activated, this, &MediaSourceTreeWidget::browseItem);

    MediaSourceTree::registerWidget(this);
}

MediaSourceTreeWidget::~MediaSourceTreeWidget()
{
    // TODO
    MediaSourceTree::unregisterWidget(this);
}

PE::BasicTreeView *MediaSourceTreeWidget::treeView()
{
    return m_view;
}

PE::BasicModel *MediaSourceTreeWidget::basicModel()
{
    return m_model;
}

PE::BasicTree *MediaSourceTreeWidget::basicTree()
{
    return MediaSourceTree::instance();
}

void MediaSourceTreeWidget::browseItem(const QModelIndex &item)
{
    PE::Node *node = m_model->nodeForIndex(item);
    MediaSourceTree::emitBrowseItem(node);
}


////////////////////////////////////////////////////////////////////////////////


MediaSourceTreeWidgetFactory::MediaSourceTreeWidgetFactory()
{
    setObjectName(QLatin1String("MediaSource.MediaSourceTreeWidgetFactory"));

    setDisplayName(tr("Media Source"));
    setPriority(110);
    setId("Media Source");
    setActivationSequence(QKeySequence(tr("Alt+M")));
}

Core::NavigationView MediaSourceTreeWidgetFactory::createWidget()
{
    Core::NavigationView nv;
    MediaSourceTreeWidget *mstw = new MediaSourceTreeWidget();
    nv.widget = mstw;
    return nv;
}

void MediaSourceTreeWidgetFactory::restoreSettings(int position, QWidget *widget)
{
    Core::INavigationWidgetFactory::restoreSettings(position, widget);
    // TODO
}

void MediaSourceTreeWidgetFactory::saveSettings(int position, QWidget *widget)
{
    Core::INavigationWidgetFactory::saveSettings(position, widget);
    // TODO
}

} // namespace Internal
} // namespace MediaSource
