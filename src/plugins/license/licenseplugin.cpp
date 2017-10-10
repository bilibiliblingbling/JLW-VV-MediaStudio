#include "licenseplugin.h"


namespace License {
namespace Internal {


LicensePlugin::LicensePlugin()
{

}

LicensePlugin::~LicensePlugin()
{

}

bool LicensePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag LicensePlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void LicensePlugin::extensionsInitialized()
{

}


} // namespace Internal
} // namespace License
