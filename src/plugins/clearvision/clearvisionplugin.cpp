#include <QWidget>
#include <utils/tmplog.h>

#include "cameratree.h"
#include "constants.h"
#include "clearvisionplugin.h"
#include "mediasource.h"
#include "mediasourcefactory.h"
#include "controlwidget.h"

#include "playerfactory.h"

namespace ClearVision {
namespace Internal {

namespace PE = ProjectExplorer;
namespace MS = MediaSource;

class ClearVisionPluginPrivate
{
public:
};


static ClearVisionPlugin *s_clearvisionplugin = 0;
static ClearVisionPluginPrivate *s_clearvisionpluginprivate = 0;

ClearVisionPlugin::ClearVisionPlugin()
{
    s_clearvisionpluginprivate = new ClearVisionPluginPrivate();
    s_clearvisionplugin = this;
}

ClearVisionPlugin::~ClearVisionPlugin()
{
    s_clearvisionplugin = 0;

    delete s_clearvisionpluginprivate;
    s_clearvisionpluginprivate = 0;
}

ClearVisionPlugin *ClearVisionPlugin::instance()
{
    return s_clearvisionplugin;
}

bool ClearVisionPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    // TODO: new mediasource factory
    new MediaSourceFactory();
    addAutoReleasedObject(new PlayerFactory());
    addAutoReleasedObject(new CameraTree());
    //ControlWidget *conwidget = new ControlWidget();
    //conwidget->show();

    return true;
}


void ClearVisionPlugin::extensionsInitialized()
{
    // TODO:
}

ExtensionSystem::IPlugin::ShutdownFlag ClearVisionPlugin::aboutToShutdown()
{
#if 0
    return AsynchronousShutdown;
#else
    return SynchronousShutdown;
#endif
}

} // namespace Internal
} // namespace ClearVision
