#include "sequencetreewidget.h"
#include "sequencewidget.h"
#include "timelineplugin.h"

#include <core/icore.h>
#include <core/pane.h>
#include <core/action/actionmanager.h>

#include <utils/tmplog.h>


namespace TimeLine {
namespace Internal {

bool TimeLinePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    TM_MARK;
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    addAutoReleasedObject(new SequenceTreeWidgetFactory());
    addAutoReleasedObject(new SequenceWidget());

    Core::ICore::timelinePane()->setWidget(SequenceWidget::instance());
    // TODO

    return true;
}

void TimeLinePlugin::extensionsInitialized()
{
    TM_MARK;
    // TODO
}

ExtensionSystem::IPlugin::ShutdownFlag TimeLinePlugin::aboutToShutdown()
{
    TM_MARK;

    // TODO

    return SynchronousShutdown;
}

} // namespace Internal
} // namespace Timeline
