#ifndef SEQUENCE_TREE_WIDGET_H
#define SEQUENCE_TREE_WIDGET_H

#include <QWidget>

#include <core/inavigationwidgetfactory.h>
#include <utils/navigationtreeview.h>

namespace TimeLine {
namespace Internal {

class SequenceTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SequenceTreeWidget(QWidget *parent = 0);
    ~SequenceTreeWidget();


private:
    QTreeView *m_view;
};


class SequenceTreeWidgetFactory : public Core::INavigationWidgetFactory
{
    Q_OBJECT

public:
    SequenceTreeWidgetFactory();

    // INavigationWidgetFactory interface
public:
    Core::NavigationView createWidget();


    void restoreSettings(int position, QWidget *widget);
    void saveSettings(int position, QWidget *widget);
};

} // namespace Internal
} // namespace TimeLine

#endif // SEQUENCE_TREE_WIDGET_H
