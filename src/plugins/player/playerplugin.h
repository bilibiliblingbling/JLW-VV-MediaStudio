#ifndef PLAYER_PLUGIN_H
#define PLAYER_PLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Player {
namespace Internal {

class PlayerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "Player.json")

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
};

} // namespace Internal
} // namespace Player

#endif // PLAYER_PLUGIN_H
