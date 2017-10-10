#include <utils/tmplog.h>

#include "genericprojectmanager.h"
#include "genericprojectplugin.h"

namespace GenericProjectManager {
namespace Internal {

bool GenericProjectPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // TODO
    addAutoReleasedObject(new GenericProjectManager());

    return true;
}

void GenericProjectPlugin::extensionsInitialized()
{
    // TODO
}

ExtensionSystem::IPlugin::ShutdownFlag GenericProjectPlugin::aboutToShutdown()
{
    // TODO

    return SynchronousShutdown;
}

} // namespace Internal
} // GenericProjectManager
