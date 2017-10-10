#ifndef LICENSE_PLUGIN_H
#define LICENSE_PLUGIN_H

#include <extensionsystem/iplugin.h>

namespace License {
namespace Internal {

class LicenseManager;
class LicensePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "License.json")

public:
    LicensePlugin();
    ~LicensePlugin();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    ShutdownFlag aboutToShutdown();
    void extensionsInitialized();

private:
    LicenseManager *m_licenseManager;
};

} // namespace Internal
} // namespace License


#endif // LICENSE_PLUGIN_H
