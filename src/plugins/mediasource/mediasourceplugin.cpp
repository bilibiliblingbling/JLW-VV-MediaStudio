#include <QAction>

#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>
#include <projectexplorer/mediasource.h>
#include <utils/tmplog.h>

#include "browsepanewidget.h"
#include "constants.h"
#include "imediasourcefactory.h"
#include "iquerytimerangefactory.h"
#include "mediasourceplugin.h"
#include "mediasourcetree.h"
#include "mediasourcetreewidget.h"
#include "segmenttree.h"

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

static MediaSourcePlugin *s_mediasourceplugin = 0;

MediaSourcePlugin::MediaSourcePlugin()
{
    s_mediasourceplugin = this;
}

MediaSourcePlugin::~MediaSourcePlugin()
{
    s_mediasourceplugin = 0;
}

MediaSourcePlugin *MediaSourcePlugin::instance()
{
    return s_mediasourceplugin;
}

bool MediaSourcePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    TM_MARK;
    // TODO

    addAutoReleasedObject(new MediaSourceTreeWidgetFactory());
    addAutoReleasedObject(new MediaSourceTree());
    addAutoReleasedObject(new SegmentTree());
    addAutoReleasedObject(new QueryTimeRangeFactoryManager());
    addAutoReleasedObject(new MediaSourceFactoryManager());
    addAutoReleasedObject(new BrowsePaneWidget());

    connect(MediaSourceTree::instance(), &MediaSourceTree::currentMediaSourceChanged,
            this, [](PE::MediaSource *mediasource) {
        SegmentTree::instance()->setCurrentMediaSource(mediasource);
        BrowsePaneWidget::setCurrentMediaSource(mediasource);
        TM_DBG << "currentMediaSourceChanged";
    });
    connect(MediaSourceTree::instance(), &MediaSourceTree::browseMediaSource,
            this, [](PE::MediaSource *mediasource) {
        BrowsePaneWidget::browseMediaSource(mediasource);
        TM_DBG << "browseMediaSource";
    });

    return true;
}

void MediaSourcePlugin::extensionsInitialized()
{
    TM_MARK;
    // TODO

    MediaSourceTree::updateMediaSourceClasses();
}

ExtensionSystem::IPlugin::ShutdownFlag MediaSourcePlugin::aboutToShutdown()
{
    TM_MARK;
    // TODO
    return SynchronousShutdown;
}



} // namespace Intenral
} // namespace MediaSource
