#ifndef MONITOR_PLUGIN_H
#define MONITOR_PLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Monitor {
namespace Internal {

class MonitorPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "Monitor.json")

public:
    MonitorPlugin();
    ~MonitorPlugin();

public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

};

} // namespace Internal
} // namespace Monitor

#endif // MONITOR_PLUGIN_H
