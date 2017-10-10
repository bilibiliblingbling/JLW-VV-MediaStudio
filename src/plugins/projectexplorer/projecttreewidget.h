#ifndef PROJECT_TREE_WIDGET_H
#define PROJECT_TREE_WIDGET_H

#include <QWidget>

#include <core/inavigationwidgetfactory.h>
#include <utils/navigationtreeview.h>

namespace ProjectExplorer {
namespace Internal {
class FlatModel;

class ProjectTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectTreeWidget(QWidget *parent = 0);
    ~ProjectTreeWidget();

    void collapseAll();

private:
    QTreeView *m_view;
    FlatModel *m_model;
};


class ProjectTreeWidgetFactory : public Core::INavigationWidgetFactory
{
    Q_OBJECT

public:
    ProjectTreeWidgetFactory();

    // INavigationWidgetFactory interface
public:
    Core::NavigationView createWidget();

    void restoreSettings(int position, QWidget *widget);
    void saveSettings(int position, QWidget *widget);
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // PROJECT_TREE_WIDGET_H
