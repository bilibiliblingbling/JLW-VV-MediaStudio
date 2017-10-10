#ifndef SEG_REPO_TREE_WIDGET_H
#define SEG_REPO_TREE_WIDGET_H

#include <QWidget>

#include <core/inavigationwidgetfactory.h>
#include <utils/navigationtreeview.h>

namespace ProjectExplorer {
namespace Internal {

class SegRepoTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SegRepoTreeWidget(QWidget *parent = 0);
    ~SegRepoTreeWidget();


private:
    QTreeView *m_view;
};


class SegRepoTreeWidgetFactory : public Core::INavigationWidgetFactory
{
    Q_OBJECT

public:
    SegRepoTreeWidgetFactory();

    // INavigationWidgetFactory interface
public:
    Core::NavigationView createWidget();


    void restoreSettings(int position, QWidget *widget);
    void saveSettings(int position, QWidget *widget);
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // SEG_REPO_TREE_WIDGET_H
