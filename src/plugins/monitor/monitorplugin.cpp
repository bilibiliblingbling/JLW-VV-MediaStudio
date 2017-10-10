#include "monitorplugin.h"
#include "monitorpanel.h"

#include <core/icore.h>
#include <core/pane.h>
#include <core/action/actionmanager.h>

namespace Monitor {
namespace Internal {

MonitorPlugin::MonitorPlugin()
{

}

MonitorPlugin::~MonitorPlugin()
{

}

bool MonitorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    addAutoReleasedObject(new MonitorPanel());
    Core::ICore::monitorPane()->setWidget(MonitorPanel::instance());

    return true;
}

void MonitorPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag MonitorPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}


} // namespace Internal
} // namespace Monitor
