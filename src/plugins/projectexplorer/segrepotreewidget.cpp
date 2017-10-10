#include "segrepotreewidget.h"


namespace ProjectExplorer {
namespace Internal {

SegRepoTreeWidget::SegRepoTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_view(0)
{
}

SegRepoTreeWidget::~SegRepoTreeWidget()
{
}


////////////////////////////////////////////////////////////////////////////////


SegRepoTreeWidgetFactory::SegRepoTreeWidgetFactory()
{
    setObjectName(QLatin1String("ProjectExplorer.SegRepoTreeWidgetFactory"));

    setDisplayName(tr("Segment Repository"));
    setPriority(120);
    setId("Segment Repository");
    setActivationSequence(QKeySequence(tr("Alt+R")));
}

Core::NavigationView SegRepoTreeWidgetFactory::createWidget()
{
    Core::NavigationView nv;
    SegRepoTreeWidget *srtw = new SegRepoTreeWidget();
    nv.widget = srtw;
    return nv;
}

void SegRepoTreeWidgetFactory::restoreSettings(int position, QWidget *widget)
{
    Core::INavigationWidgetFactory::restoreSettings(position, widget);
    // TODO
}

void SegRepoTreeWidgetFactory::saveSettings(int position, QWidget *widget)
{
    Core::INavigationWidgetFactory::saveSettings(position, widget);
    // TODO
}


} // namespace Internal
} // namespace ProjectExplorer
