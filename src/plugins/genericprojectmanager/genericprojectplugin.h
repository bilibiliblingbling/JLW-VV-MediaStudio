#ifndef GENERIC_PROJECT_PLUGIN_H
#define GENERIC_PROJECT_PLUGIN_H

#include <extensionsystem/iplugin.h>

namespace GenericProjectManager {
namespace Internal {

class GenericProjectPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "GenericProjectManager.json")

public:


    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
};

} // namespace Internal
} // namespace GenericProjectManager

#endif // GENERIC_PROJECT_PLUGIN_H
