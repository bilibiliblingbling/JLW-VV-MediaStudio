#include "sequencetreewidget.h"

namespace TimeLine {
namespace Internal {

SequenceTreeWidget::SequenceTreeWidget(QWidget *parent)
    : QWidget(parent)
    , m_view(0)
{
}

SequenceTreeWidget::~SequenceTreeWidget()
{
}


////////////////////////////////////////////////////////////////////////////////


SequenceTreeWidgetFactory::SequenceTreeWidgetFactory()
{
    setObjectName(QLatin1String("TimeLine.SequenceTreeWidgetFactory"));

    setDisplayName(tr("Sequence"));
    setPriority(130);
    setId("Sequence");
    setActivationSequence(QKeySequence(tr("Alt+S")));
}

Core::NavigationView SequenceTreeWidgetFactory::createWidget()
{
    Core::NavigationView nv;
    SequenceTreeWidget *stw = new SequenceTreeWidget();
    nv.widget = stw;
    return nv;
}

void SequenceTreeWidgetFactory::restoreSettings(int position, QWidget *widget)
{
    Core::INavigationWidgetFactory::restoreSettings(position, widget);
    // TODO
}

void SequenceTreeWidgetFactory::saveSettings(int position, QWidget *widget)
{
    Core::INavigationWidgetFactory::saveSettings(position, widget);
    // TODO
}

} // namespace Internal
} // namespace TimeLine
