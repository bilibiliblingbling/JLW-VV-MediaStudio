#ifndef NAVIGATION_WIDGET_H
#define NAVIGATION_WIDGET_H

#include "core_global.h"
#include "minisplitter.h"
#include "id.h"

QT_BEGIN_NAMESPACE
class QAction;
class QAbstractItemModel;
class QSettings;
QT_END_NAMESPACE

namespace Core {

class Command;
class NavigationWidget;
class NavigationWidgetPrivate;
class IMode;
class INavigationWidgetFactory;

namespace Internal { class NavigationSubWidget; }

class CORE_EXPORT NavigationWidgetPlaceHolder : public QWidget
{
    Q_OBJECT

    friend class Core::NavigationWidget;

public:
    explicit NavigationWidgetPlaceHolder(IMode *mode, QWidget *parent = 0);
    ~NavigationWidgetPlaceHolder();

    static NavigationWidgetPlaceHolder* current();
    void applyStoredSize(int width);

private:
    void currentModeAboutToChange(IMode *mode);

private:
    IMode *m_mode;
    static NavigationWidgetPlaceHolder *m_current;
};


class CORE_EXPORT NavigationWidget : public MiniSplitter
{
    Q_OBJECT

public:
    enum FactoryModelRoles {
        FactoryObjectRole = Qt::UserRole,
        FactoryIdRole,
        FactoryPriorityRole
    };

public:
    explicit NavigationWidget(QAction *toggleSideBarAction);
    ~NavigationWidget();

    static NavigationWidget* instance();

    void setFactories(const QList<INavigationWidgetFactory*> &factories);

    void saveSettings(QSettings *settings);
    void restoreSettings(QSettings *settings);

    QWidget *activateSubWidget(Id factoryId);
    void closeSubWidgets();

    bool isShown() const;
    void setShown(bool b);

    bool isSuppressed() const;
    void setSuppressed(bool b);

    int storedWidth();

    // Called from the place holders
    void placeHolderChanged(NavigationWidgetPlaceHolder *holder);

    QHash<Id, Command *> commandMap() const;
    QAbstractItemModel *factoryModel() const;

protected:
    void resizeEvent(QResizeEvent *re);

private slots:
    void activateSubWidget();
    void splitSubWidget(int factoryIndex);
    void closeSubWidget();

private:
    void updateToggleText();
    Internal::NavigationSubWidget *insertSubItem(int position, int index);
    int factoryIndex(Id id);

private:
    NavigationWidgetPrivate *d;
};

} // namespace Core

#endif // NAVIGATION_WIDGET_H
