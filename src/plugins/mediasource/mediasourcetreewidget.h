#ifndef MEDIA_SOURCE_TREE_WIDGET_H
#define MEDIA_SOURCE_TREE_WIDGET_H

#include <core/inavigationwidgetfactory.h>
#include <projectexplorer/basictreewidget.h>

namespace MediaSource {
namespace Internal {

class MediaSourceTreeWidget : public ProjectExplorer::BasicTreeWidget
{
    Q_OBJECT

public:
    explicit MediaSourceTreeWidget(QWidget *parent = 0);
    ~MediaSourceTreeWidget();

private:
    ProjectExplorer::BasicTreeView *treeView() Q_DECL_OVERRIDE;
    ProjectExplorer::BasicModel *basicModel() Q_DECL_OVERRIDE;
    ProjectExplorer::BasicTree *basicTree() Q_DECL_OVERRIDE;

private slots:
    void browseItem(const QModelIndex &item);

private:
    ProjectExplorer::BasicTreeView *m_view;
    ProjectExplorer::BasicModel *m_model;
};


class MediaSourceTreeWidgetFactory : public Core::INavigationWidgetFactory
{
    Q_OBJECT

public:
    MediaSourceTreeWidgetFactory();

    // INavigationWidgetFactory interface
public:
    Core::NavigationView createWidget();


    void restoreSettings(int position, QWidget *widget);
    void saveSettings(int position, QWidget *widget);
};

} // namespace Internal
} // namespace MediaSource

#endif // MEDIA_SOURCE_TREE_WIDGET_H
