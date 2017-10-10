#include "corelistener.h"
#include "projectexplorerplugin.h"

namespace ProjectExplorer {
namespace Internal {

CoreListener::CoreListener()
{
    setObjectName(QLatin1String("ProjectExplorer.CoreListener"));
}

bool CoreListener::coreAboutToClose()
{
    return ProjectExplorerPlugin::instance()->coreAboutToClose();
}

} // namespace Internal
} // namespace ProjectExplorer
