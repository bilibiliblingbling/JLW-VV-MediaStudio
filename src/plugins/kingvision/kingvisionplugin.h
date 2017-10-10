#ifndef KINGVISION_PLUGIN_H
#define KINGVISION_PLUGIN_H

#include <extensionsystem/iplugin.h>

namespace KingVision {
namespace Internal {

class KingVisionPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "KingVision.json")

public:
    KingVisionPlugin();
    ~KingVisionPlugin();

    static KingVisionPlugin *instance();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_PLUGIN_H
