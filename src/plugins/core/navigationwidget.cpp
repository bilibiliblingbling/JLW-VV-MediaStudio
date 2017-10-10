#include <QAction>
#include <QCoreApplication>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include "action/actionmanager.h"
#include "constants.h"
#include "icontext.h"
#include "icore.h"
#include "imode.h"
#include "inavigationwidgetfactory.h"
#include "modemanager.h"
#include "navigationsubwidget.h"
#include "navigationwidget.h"


namespace Core {

NavigationWidgetPlaceHolder *NavigationWidgetPlaceHolder::m_current = 0;

NavigationWidgetPlaceHolder::NavigationWidgetPlaceHolder(IMode *mode, QWidget *parent)
    : QWidget(parent)
    , m_mode(mode)
{
    setLayout(new QVBoxLayout());
    layout()->setMargin(0);
    connect(ModeManager::instance(), &ModeManager::currentModeAboutToChange,
            this, &NavigationWidgetPlaceHolder::currentModeAboutToChange);
}

NavigationWidgetPlaceHolder::~NavigationWidgetPlaceHolder()
{
    if (m_current == this) {
        if (NavigationWidget *nw = NavigationWidget::instance()) {
            nw->setParent(0);
            nw->hide();
        }
    }
}

NavigationWidgetPlaceHolder *NavigationWidgetPlaceHolder::current()
{
    return m_current;
}

void NavigationWidgetPlaceHolder::applyStoredSize(int width)
{
    if (width) {
        QSplitter *splitter = qobject_cast<QSplitter *>(parentWidget());
        if (splitter) {
            // A splitter we need to resize the splitter sizes
            QList<int> sizes = splitter->sizes();
            int index = splitter->indexOf(this);
            int diff = width - sizes.at(index);
            int adjust = sizes.count() > 1 ? (diff / (sizes.count() - 1)) : 0;
            for (int i = 0; i < sizes.count(); ++i) {
                if (i != index)
                    sizes[i] += adjust;
            }
            sizes[index]= width;
            splitter->setSizes(sizes);
        } else {
            QSize s = size();
            s.setWidth(width);
            resize(s);
        }
    }
}

void NavigationWidgetPlaceHolder::currentModeAboutToChange(IMode *mode)
{
    NavigationWidget *navigationWidget = NavigationWidget::instance();

    if (m_current == this) {
        m_current = 0;
        navigationWidget->setParent(0);
        navigationWidget->hide();
        navigationWidget->placeHolderChanged(m_current);
    }
    if (m_mode == mode) {
        m_current = this;

        int width = navigationWidget->storedWidth();

        layout()->addWidget(navigationWidget);
        navigationWidget->show();

        applyStoredSize(width);
        setVisible(navigationWidget->isShown());
        navigationWidget->placeHolderChanged(m_current);
    }
}


class NavigationWidgetPrivate
{
public:
    explicit NavigationWidgetPrivate(QAction *toggleSideBarAction)
        : m_factoryModel(new QStandardItemModel)
        , m_shown(true)
        , m_suppressed(false)
        , m_width(0)
        , m_toggleSideBarAction(toggleSideBarAction)
    {
    }

    ~NavigationWidgetPrivate() { delete m_factoryModel; }

public:
    QStandardItemModel *m_factoryModel;

    bool m_shown;
    bool m_suppressed;
    int m_width;

    QAction *m_toggleSideBarAction; // does not take ownership

public:
    QList<Internal::NavigationSubWidget *> m_subWidgets;
    QHash<QAction *, Id> m_actionMap;
    QHash<Id, Command *> m_commandMap;

public:

    static NavigationWidget* m_instance;
};
NavigationWidget *NavigationWidgetPrivate::m_instance = 0;


NavigationWidget::NavigationWidget(QAction *toggleSideBarAction)
    : d(new NavigationWidgetPrivate(toggleSideBarAction))
{
    d->m_factoryModel->setSortRole(FactoryPriorityRole);
    setOrientation(Qt::Vertical);
    d->m_instance = this;

    setMinimumWidth(240);
}

NavigationWidget::~NavigationWidget()
{
    NavigationWidgetPrivate::m_instance = 0;
    delete d;
}

NavigationWidget *NavigationWidget::instance()
{
    return NavigationWidgetPrivate::m_instance;
}

void NavigationWidget::setFactories(const QList<INavigationWidgetFactory *> &factories)
{
    Context naviContext(Constants::C_NAVIGATION_PANE);

    foreach (INavigationWidgetFactory *factory, factories) {
        const Id id = factory->id();

        QAction *action = new QAction(tr("Activate %1 View").arg(factory->displayName()), this);
        connect(action, SIGNAL(triggered()), this, SLOT(activateSubWidget()));
        d->m_actionMap.insert(action, id);

        Command *cmd = ActionManager::registerAction(action,
                                                     id.withPrefix("MediaStudio.Sidebar."),
                                                     naviContext);
        cmd->setDefaultKeySequence(factory->activationSequence());
        d->m_commandMap.insert(id, cmd);

        QStandardItem *newRow = new QStandardItem(factory->displayName());
        newRow->setData(qVariantFromValue(factory), FactoryObjectRole);
        newRow->setData(QVariant::fromValue(factory->id()), FactoryIdRole);
        newRow->setData(factory->priority(), FactoryPriorityRole);
        d->m_factoryModel->appendRow(newRow);
    }
    d->m_factoryModel->sort(0);
    updateToggleText();
}

void NavigationWidget::saveSettings(QSettings *settings)
{
    QStringList viewIds;
    for (int i = 0; i < d->m_subWidgets.count(); ++i) {
        d->m_subWidgets.at(i)->saveSettings();
        viewIds.append(d->m_subWidgets.at(i)->factory()->id().toString());
    }
    settings->setValue(QLatin1String("Navigation/Views"), viewIds);
    settings->setValue(QLatin1String("Navigation/Visible"), isShown());
    settings->setValue(QLatin1String("Navigation/VerticalPosition"), saveState());
    settings->setValue(QLatin1String("Navigation/Width"), d->m_width);
}

void NavigationWidget::restoreSettings(QSettings *settings)
{
    if (!d->m_factoryModel->rowCount()) {
        // We have no widgets to show!
        setShown(false);
        return;
    }

    QStringList viewIds = settings->value(QLatin1String("Navigation/Views"),
                                          QStringList(QLatin1String("Projects"))).toStringList();

    bool restoreSplitterState = true;

    int position = 0;
    foreach (const QString &id, viewIds) {
        int index = factoryIndex(Id::fromString(id));
        if (index >= 0) {
            // Only add if the id was actually found!
            insertSubItem(position, index);
            ++position;
        } else {
            restoreSplitterState = false;
        }
    }

    if (d->m_subWidgets.isEmpty()) {
        // Make sure we have at least the projects widget
        insertSubItem(0, qMax(0, factoryIndex("Projects")));
    }

    setShown(settings->value(QLatin1String("Navigation/Visible"), true).toBool());

    if (restoreSplitterState && settings->contains(QLatin1String("Navigation/VerticalPosition"))) {
        restoreState(settings->value(QLatin1String("Navigation/VerticalPosition")).toByteArray());
    } else {
        QList<int> sizes;
        sizes += 256;
        for (int i = viewIds.size()-1; i > 0; --i) {
            sizes.prepend(512);
        }
        setSizes(sizes);
    }

    d->m_width = settings->value(QLatin1String("Navigation/Width"), 240).toInt();
    if (d->m_width < 40)
        d->m_width = 40;

    // Apply
    if (NavigationWidgetPlaceHolder::m_current)
        NavigationWidgetPlaceHolder::m_current->applyStoredSize(d->m_width);
}

QWidget *NavigationWidget::activateSubWidget(Id factoryId)
{
    setShown(true);

    foreach (Internal::NavigationSubWidget *subWidget, d->m_subWidgets) {
        if (subWidget->factory()->id() == factoryId) {
            subWidget->setFocusWidget();
            ICore::raiseWindow(this);
            return subWidget->widget();
        }
    }

    int index = factoryIndex(factoryId);
    if (index >= 0) {
        d->m_subWidgets.first()->setFactoryIndex(index);
        d->m_subWidgets.first()->setFocusWidget();
        ICore::raiseWindow(this);
        return d->m_subWidgets.first()->widget();
    }

    return 0;
}

void NavigationWidget::closeSubWidgets()
{
    foreach (Internal::NavigationSubWidget *subWidget, d->m_subWidgets) {
        subWidget->saveSettings();
        delete subWidget;
    }
    d->m_subWidgets.clear();
}

bool NavigationWidget::isShown() const
{
    return d->m_shown;
}

void NavigationWidget::setShown(bool b)
{
    if (d->m_shown == b)
        return;

    bool haveData = d->m_factoryModel->rowCount();
    d->m_shown = b;
    if (NavigationWidgetPlaceHolder::m_current) {
        b = (d->m_shown && !d->m_suppressed && haveData);
        NavigationWidgetPlaceHolder::m_current->setVisible(b);
        d->m_toggleSideBarAction->setChecked(b);
    } else {
        d->m_toggleSideBarAction->setChecked(false);
    }
    updateToggleText();
}

bool NavigationWidget::isSuppressed() const
{
    return d->m_suppressed;
}

void NavigationWidget::setSuppressed(bool b)
{
    if (d->m_suppressed == b)
        return;

    d->m_suppressed = b;
    if (NavigationWidgetPlaceHolder::m_current)
        NavigationWidgetPlaceHolder::m_current->setVisible(d->m_shown && !d->m_suppressed);
}

int NavigationWidget::storedWidth()
{
    return d->m_width;
}

void NavigationWidget::placeHolderChanged(NavigationWidgetPlaceHolder *holder)
{
    d->m_toggleSideBarAction->setChecked(holder && isShown());
    updateToggleText();
}

QHash<Id, Command *> NavigationWidget::commandMap() const
{
    return d->m_commandMap;
}

QAbstractItemModel *NavigationWidget::factoryModel() const
{
    return d->m_factoryModel;
}

void NavigationWidget::resizeEvent(QResizeEvent *re)
{
    if (d->m_width && re->size().width())
        d->m_width = re->size().width();

    MiniSplitter::resizeEvent(re);
}

void NavigationWidget::activateSubWidget()
{
    QAction *original = qobject_cast<QAction *>(sender());
    Id id = d->m_actionMap[original];
    activateSubWidget(id);
}

void NavigationWidget::splitSubWidget(int factoryIndex)
{
    Internal::NavigationSubWidget *original = qobject_cast<Internal::NavigationSubWidget *>(sender());
    int pos = indexOf(original) + 1;
    insertSubItem(pos, factoryIndex);
}

void NavigationWidget::closeSubWidget()
{
    if (d->m_subWidgets.count() != 1) {
        Internal::NavigationSubWidget *subWidget = qobject_cast<Internal::NavigationSubWidget *>(sender());
        subWidget->saveSettings();
        d->m_subWidgets.removeOne(subWidget);
        subWidget->hide();
        subWidget->deleteLater();

        // TODO: icons
#if 0
        // update close button of top item
        if (d->m_subWidgets.size() == 1)
            d->m_subWidgets.at(0)->setCloseIcon(QIcon(QLatin1String(Constants::ICON_CLOSE_SPLIT_LEFT)));
        else
            d->m_subWidgets.at(0)->setCloseIcon(QIcon(QLatin1String(Constants::ICON_CLOSE_SPLIT_TOP)));
#endif
    } else {
        setShown(false);
    }
}

void NavigationWidget::updateToggleText()
{
    bool haveData = d->m_factoryModel->rowCount();
    d->m_toggleSideBarAction->setVisible(haveData);
    d->m_toggleSideBarAction->setEnabled(haveData && NavigationWidgetPlaceHolder::m_current);

    // TODO: text
    d->m_toggleSideBarAction->setToolTip(
                QCoreApplication::translate("Core",
                                            isShown()? Constants::TR_HIDE_SIDEBAR
                                                     : Constants::TR_SHOW_SIDEBAR));
}

Internal::NavigationSubWidget *NavigationWidget::insertSubItem(int position, int index)
{
    for (int pos = position + 1; pos < d->m_subWidgets.size(); ++pos) {
        d->m_subWidgets.at(pos)->setPosition(pos + 1);
    }

    // TODO: update icons
#if 0
    if (!d->m_subWidgets.isEmpty()) // Make all icons the bottom icon
        d->m_subWidgets.at(0)->setCloseIcon(QIcon(QLatin1String(Constants::ICON_CLOSE_SPLIT_BOTTOM)));
#endif

    Internal::NavigationSubWidget *nsw = new Internal::NavigationSubWidget(this, position, index);
    connect(nsw, &Internal::NavigationSubWidget::splitMe,
            this, &NavigationWidget::splitSubWidget);
    connect(nsw, SIGNAL(closeMe()), this, SLOT(closeSubWidget()));
    insertWidget(position, nsw);
    d->m_subWidgets.insert(position, nsw);
#if 0
    if (d->m_subWidgets.size() == 1)
        d->m_subWidgets.at(0)->setCloseIcon(QIcon(QLatin1String(Constants::ICON_CLOSE_SPLIT_LEFT)));
    else
        d->m_subWidgets.at(0)->setCloseIcon(QIcon(QLatin1String(Constants::ICON_CLOSE_SPLIT_TOP)));
#endif
    return nsw;
}

int NavigationWidget::factoryIndex(Id id)
{
    for (int row = 0; row < d->m_factoryModel->rowCount(); ++row) {
        if (d->m_factoryModel->data(d->m_factoryModel->index(row, 0), FactoryIdRole).value<Id>() == id)
            return row;
    }

    return -1;
}





// namespace Internal
} // namespace Core
