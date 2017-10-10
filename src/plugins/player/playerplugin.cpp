#include "playerplugin.h"

#include <share/SoftAuthorizer/StreamPlayGAuthorHelper.h>

namespace Player {
namespace Internal {

bool PlayerPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // TODO:
    ::StreamPlayGAuthorHelper::Authorize();

    return true;
}

void PlayerPlugin::extensionsInitialized()
{
    // TODO:
}

ExtensionSystem::IPlugin::ShutdownFlag PlayerPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

} // namespace Internal
} // namespace Player
