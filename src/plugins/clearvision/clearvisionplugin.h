#ifndef CLEARVISION_PLUGIN_H
#define CLEARVISION_PLUGIN_H

#include <extensionsystem/iplugin.h>

namespace ClearVision {
namespace Internal {

class ClearVisionPlugin : public  ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "ClearVision.json")

public:
    ClearVisionPlugin();
    ~ClearVisionPlugin();

    static ClearVisionPlugin *instance();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_PLUGIN_H
