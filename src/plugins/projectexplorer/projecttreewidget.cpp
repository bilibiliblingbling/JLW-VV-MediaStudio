#include <QStyledItemDelegate>

#include <core/icore.h>
#include <utils/qtcassert.h>

#include "constants.h"
#include "project.h"
#include "projecttreewidget.h"


namespace ProjectExplorer {
namespace Internal {

////////////////////////////////////////////////////////////////////////////////

class ProjectTreeItemDelegate : public QStyledItemDelegate
{
public:
    ProjectTreeItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
    {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyleOptionViewItem opt = option;
        if (!index.data(Project::EnabledRole).toBool())
            opt.state &= ~QStyle::State_Enabled;
        QStyledItemDelegate::paint(painter, opt, index);
    }
};

class ProjectTreeView : public Utils::NavigationTreeView
{
public:
    ProjectTreeView()
    {
        setEditTriggers(QAbstractItemView::EditKeyPressed);
        setContextMenuPolicy(Qt::CustomContextMenu);
        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);

        m_context = new Core::IContext(this);
        m_context->setContext(Core::Context(ProjectExplorer::Constants::C_PROJECT_TREE_VIEW));
        m_context->setWidget(this);
        Core::ICore::addContextObject(m_context);
    }
    ~ProjectTreeView()
    {
        Core::ICore::removeContextObject(m_context);
        delete m_context;
    }

private:
    Core::IContext *m_context;
};

////////////////////////////////////////////////////////////////////////////////


ProjectTreeWidget::ProjectTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_view(0)
    , m_model(0)
{
}

ProjectTreeWidget::~ProjectTreeWidget()
{

}

////////////////////////////////////////////////////////////////////////////////


ProjectTreeWidgetFactory::ProjectTreeWidgetFactory()
{
    setDisplayName(tr("Projects"));
    setPriority(100);
    setId("Projects");
    setActivationSequence(QKeySequence(tr("Alt+X")));
}

Core::NavigationView ProjectTreeWidgetFactory::createWidget()
{
    Core::NavigationView nv;
    ProjectTreeWidget *ptw = new ProjectTreeWidget();
    nv.widget = ptw;
    return nv;
}

void ProjectTreeWidgetFactory::restoreSettings(int position, QWidget *widget)
{
#if 0
    ProjectTreeWidget *ptw = qobject_cast<ProjectTreeWidget *>(widget);
    Q_ASSERT(ptw);
    QSettings *settings = Core::ICore::settings();
    const QString baseKey = QLatin1String("ProjectTreeWidget.") + QString::number(position);
    ptw->setProjectFilter(settings->value(baseKey + QLatin1String(".ProjectFilter"), false).toBool());
    ptw->setGeneratedFilesFilter(settings->value(baseKey + QLatin1String(".GeneratedFilter"), true).toBool());
#endif
}

void ProjectTreeWidgetFactory::saveSettings(int position, QWidget *widget)
{
#if 0
    ProjectTreeWidget *ptw = qobject_cast<ProjectTreeWidget *>(widget);
    Q_ASSERT(ptw);
    QSettings *settings = Core::ICore::settings();
    const QString baseKey = QLatin1String("ProjectTreeWidget.") + QString::number(position);
    settings->setValue(baseKey + QLatin1String(".ProjectFilter"), ptw->projectFilter());
    settings->setValue(baseKey + QLatin1String(".GeneratedFilter"), ptw->generatedFilesFilter());
#endif
}



} // namespace Internal
} // namespace ProjectExplorer
