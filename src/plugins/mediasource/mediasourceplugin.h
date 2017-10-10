#ifndef MEDIA_SOURCE_PLUGIN_H
#define MEDIA_SOURCE_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include "mediasource_global.h"

namespace MediaSource {
namespace Internal {

class MediaSourcePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "MediaSource.json")

public:
    MediaSourcePlugin();
    ~MediaSourcePlugin();

    static MediaSourcePlugin *instance();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
};

} // namespace Internals
} // namespace MediaSource


#endif // MEDIA_SOURCE_PLUGIN_H
