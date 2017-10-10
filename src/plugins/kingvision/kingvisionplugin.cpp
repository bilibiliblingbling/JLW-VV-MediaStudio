#include <QAction>
#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>
#include <mediasource/constants.h>
#include <utils/tmplog.h>

#include "cameratree.h"
#include "constants.h"
#include "devicetree.h"
#include "mediasource.h"
#include "mediasourcefactory.h"
#include "kingvisionplugin.h"
#include "playerfactory.h"

namespace KingVision {
namespace Internal {

namespace PE = ProjectExplorer;
namespace MS = MediaSource;

static KingVisionPlugin *s_kingvisionplugin = 0;

KingVisionPlugin::KingVisionPlugin()
{
    s_kingvisionplugin = this;
}

KingVisionPlugin::~KingVisionPlugin()
{
    s_kingvisionplugin = 0;
}

KingVisionPlugin *KingVisionPlugin::instance()
{
    return s_kingvisionplugin;
}

bool KingVisionPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    TM_MARK;

    // TODO: add new media source factory
    addAutoReleasedObject(new DeviceTree());
    addAutoReleasedObject(new CameraTree());

#if 0
    addAutoReleasedObject(new PlayerFactory());
#else
    addAutoReleasedObject(new CameraPlayerFactory());
    addAutoReleasedObject(new SegmentPlayerFactory());
#endif


    // will be freed by \c MediaSourceFactoryManager
    new MediaSourceFactory();

    return true;
}

void KingVisionPlugin::extensionsInitialized()
{
    // TODO
}

ExtensionSystem::IPlugin::ShutdownFlag KingVisionPlugin::aboutToShutdown()
{
    // TODO
#if 0
    return AsynchronousShutdown;
#else
    return SynchronousShutdown;
#endif
}

} // namespace Internal
} // namespace KingVision
