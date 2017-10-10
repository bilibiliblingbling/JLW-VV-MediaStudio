#ifndef TIME_LINE_PLUGIN_H
#define TIME_LINE_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include "timeline_global.h"

namespace TimeLine {
namespace Internal {

class TimeLinePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "TimeLine.json")

public:

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
};

} // namespace Internal
} // namespace TimeLine

#endif // TIME_LINE_PLUGIN_H
