#ifndef NAVIGATION_SUB_WIDGET_H
#define NAVIGATION_SUB_WIDGET_H

#include <QList>
#include <QWidget>

#include "commandcombobox.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QToolButton;
QT_END_NAMESPACE

namespace Utils { class StyledBar; }

namespace Core {

class INavigationWidgetFactory;
class Command;
class NavigationWidget;

namespace Internal {

class NavigationSubWidget : public QWidget
{
    Q_OBJECT

public:
    NavigationSubWidget(NavigationWidget *parentWidget, int position, int index);
    ~NavigationSubWidget();

    INavigationWidgetFactory *factory();

    int factoryIndex() const;
    void setFactoryIndex(int i);

    void setFocusWidget();

    int position() const;
    void setPosition(int position);

    void saveSettings();
    void restoreSettings();

    Command *command(const QString &title) const;
    void setCloseIcon(const QIcon &icon);

    QWidget *widget();

signals:
    void splitMe(int factoryIndex);
    void closeMe();

private slots:
    void comboBoxIndexChanged(int factoryIndex);

private:
    void populateSplitMenu();

private:
    NavigationWidget *m_parentWidget;
    QComboBox *m_navigationComboBox;
    Utils::StyledBar *m_toolBar;
    QMenu *m_splitMenu;
    QToolButton *m_closeButton;

    QWidget *m_navigationWidget;
    INavigationWidgetFactory *m_navigationWidgetFactory;
    QList<QToolButton *> m_additionalToolBarWidgets;

    int m_position;
};

class NavComboBox : public CommandComboBox
{
    Q_OBJECT

public:
    explicit NavComboBox(NavigationSubWidget *navSubWidget);

private:
    const Command *command(const QString &text) const;

private:
    NavigationSubWidget *m_navSubWidget;
};

} // namespace Internal
} // namespace Core

#endif // NAVIGATION_SUB_WIDGET_H
