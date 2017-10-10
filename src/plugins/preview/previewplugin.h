#ifndef PREVIEW_PLUGIN_H
#define PREVIEW_PLUGIN_H

#include <extensionsystem/iplugin.h>
#include <QAction>

#include "previewpanel.h"

namespace Preview {
namespace Internal {


/*!
  * @brief implement the base class IPlugin,
  *         concrete message given by iplugin.cpp
 **/
class PreviewPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "Preview.json")

public:
    PreviewPlugin();
    ~PreviewPlugin();

    static PreviewPlugin *instance();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

};

} // namespace Internal
} // namespace Preview



#endif // PREVIEWPLUGIN_H
