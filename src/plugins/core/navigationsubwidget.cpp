#include <QCoreApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QMenu>
#include <QToolButton>

#include <utils/styledbar.h>

#include "action/command.h"
#include "constants.h"
#include "inavigationwidgetfactory.h"
#include "navigationsubwidget.h"
#include "navigationwidget.h"


namespace Core {
namespace Internal {

NavigationSubWidget::NavigationSubWidget(NavigationWidget *parentWidget, int position, int index)
    : m_parentWidget(parentWidget)
    , m_navigationComboBox(new NavComboBox(this))
    , m_toolBar(new Utils::StyledBar(this))
    , m_splitMenu(0)
    , m_closeButton(new QToolButton())
    , m_navigationWidget(0)
    , m_navigationWidgetFactory(0)
    , m_position(position)
{
    m_navigationComboBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_navigationComboBox->setFocusPolicy(Qt::TabFocus);
    m_navigationComboBox->setMinimumContentsLength(0);
    m_navigationComboBox->setModel(parentWidget->factoryModel());
    m_navigationComboBox->setMaximumHeight(24);

    QHBoxLayout *toolBarLayout = new QHBoxLayout;
    toolBarLayout->setMargin(0);
    toolBarLayout->setSpacing(0);
    m_toolBar->setLayout(toolBarLayout);
    toolBarLayout->addWidget(m_navigationComboBox);

    QToolButton *splitAction = new QToolButton();
    // TODO
//    splitAction->setIcon(QIcon(QLatin1String(Constants::ICON_SPLIT_HORIZONTAL)));
    splitAction->setArrowType(Qt::RightArrow);
    splitAction->setToolTip(tr("Split"));
    splitAction->setPopupMode(QToolButton::InstantPopup);
    splitAction->setProperty("noArrow", true);
    m_splitMenu = new QMenu(splitAction);
    splitAction->setMenu(m_splitMenu);
    connect(m_splitMenu, &QMenu::aboutToShow, this, &NavigationSubWidget::populateSplitMenu);

    // TODO
    m_closeButton->setIcon(QIcon(QLatin1String(Constants::ICON_CLOSE_BUTTON)));
    m_closeButton->setToolTip(QCoreApplication::translate("Core", Constants::TR_CLOSE));

    toolBarLayout->addWidget(splitAction);
    toolBarLayout->addWidget(m_closeButton);

    QVBoxLayout *lay = new QVBoxLayout();
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);
    lay->addWidget(m_toolBar);

    connect(m_closeButton, SIGNAL(clicked()), this, SIGNAL(closeMe()));

    setFactoryIndex(index);

    connect(m_navigationComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboBoxIndexChanged(int)));

    comboBoxIndexChanged(index);
}

NavigationSubWidget::~NavigationSubWidget()
{
}

INavigationWidgetFactory *NavigationSubWidget::factory()
{
    return m_navigationWidgetFactory;
}

void NavigationSubWidget::setFactoryIndex(int i)
{
    m_navigationComboBox->setCurrentIndex(i);
}

void NavigationSubWidget::setFocusWidget()
{
    if (m_navigationWidget)
        m_navigationWidget->setFocus();
}

int NavigationSubWidget::position() const
{
    return m_position;
}

void NavigationSubWidget::setPosition(int position)
{
    m_position = position;
}

void NavigationSubWidget::saveSettings()
{
    if (!m_navigationWidget || !factory())
        return;

    factory()->saveSettings(position(), m_navigationWidget);
}

void NavigationSubWidget::restoreSettings()
{

}

Command *NavigationSubWidget::command(const QString &title) const
{
    const QHash<Id, Command *> commandMap = m_parentWidget->commandMap();
    QHash<Id, Command *>::const_iterator r = commandMap.find(Id::fromString(title));
    if (r != commandMap.end())
        return r.value();

    return 0;
}

QWidget *NavigationSubWidget::widget()
{
    return m_navigationWidget;
}


void NavigationSubWidget::comboBoxIndexChanged(int factoryIndex)
{
    saveSettings();

    // Remove toolbutton
    foreach (QWidget *w, m_additionalToolBarWidgets) {
        delete w;
    }

    m_additionalToolBarWidgets.clear();

    // Remove old Widget
    delete m_navigationWidget;
    m_navigationWidget = 0;
    m_navigationWidgetFactory = 0;
    if (factoryIndex == -1)
        return;

    // Get new stuff
    m_navigationWidgetFactory = m_navigationComboBox->itemData(factoryIndex,
                           NavigationWidget::FactoryObjectRole).value<INavigationWidgetFactory *>();
    NavigationView n = m_navigationWidgetFactory->createWidget();
    m_navigationWidget = n.widget;
    layout()->addWidget(m_navigationWidget);

    // Add Toolbutton
    m_additionalToolBarWidgets = n.dockToolBarWidgets;
    QHBoxLayout *layout = qobject_cast<QHBoxLayout *>(m_toolBar->layout());
    foreach (QToolButton *w, m_additionalToolBarWidgets) {
        layout->insertWidget(layout->count()-2, w);
    }

    restoreSettings();
}

void NavigationSubWidget::populateSplitMenu()
{
    m_splitMenu->clear();

    QAbstractItemModel *factoryModel = m_parentWidget->factoryModel();
    int count = factoryModel->rowCount();
    for (int i = 0; i < count; ++i) {
        QModelIndex index = factoryModel->index(i, 0);
        QAction *action = m_splitMenu->addAction(factoryModel->data(index).toString());
        connect(action, &QAction::triggered, this, [this, i]() { emit splitMe(i); });
    }
}


NavComboBox::NavComboBox(NavigationSubWidget *navSubWidget)
    : m_navSubWidget(navSubWidget)
{}

const Command *NavComboBox::command(const QString &text) const
{
    return m_navSubWidget->command(text);
}

} // namespace Internal
} // namespace Core
