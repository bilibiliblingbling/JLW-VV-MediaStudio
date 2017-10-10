#include <QAction>
#include <QCoreApplication>
#include <QDebug>
#include <QMenu>
#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>
#include <core/icore.h>
#include <core/icore.h>

#include "constants.h"
#include "ipreviewlayoutfactory.h"
#include "previewplugin.h"

namespace Preview {
namespace Internal {

class PreviewPluginPrivate : public QObject
{
    Q_DECLARE_TR_FUNCTIONS(Preview::Internal::PreviewPlugin)
public:
    PreviewPluginPrivate();

public:
    void loadPersistentSettings();

public:
    QMenu *m_layoutMenu;

};

static PreviewPluginPrivate *dd = 0;
static PreviewPlugin *s_previewplugin = 0;

PreviewPlugin::PreviewPlugin()
{
    dd = new PreviewPluginPrivate();
    s_previewplugin = this;
}

PreviewPlugin::~PreviewPlugin()
{
    s_previewplugin = 0;
    delete dd;
    dd = 0;
}

PreviewPlugin *PreviewPlugin::instance()
{
    return s_previewplugin;
}

bool PreviewPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);

    addAutoReleasedObject(new PreviewLayoutFactoryManager());
    addAutoReleasedObject(new PreviewPanel());

    Core::ActionContainer * windowsmenu =
            Core::ActionManager::actionContainer(Core::Constants::M_WINDOW);
    // add group
    windowsmenu->appendGroup(Constants::G_WINDOW_PREVIEWLAYOUTS);
    windowsmenu->addSeparator(Constants::G_WINDOW_PREVIEWLAYOUTS);
    // create Menu
    Core::ActionContainer *previewlayoutmenu =
            Core::ActionManager::createMenu(Constants::M_PREVIEW_PREVIEWLAYOUTS);
    previewlayoutmenu->menu()->setTitle(tr("&PreviewLayouts"));
    previewlayoutmenu->setOnAllDisabledBehavior(Core::ActionContainer::Show);
    windowsmenu->addMenu(previewlayoutmenu, Constants::G_WINDOW_PREVIEWLAYOUTS);
    dd->m_layoutMenu = previewlayoutmenu->menu();
    // add a group for action

    connect(windowsmenu->menu(), &QMenu::aboutToShow,
            dd, []() { dd->m_layoutMenu->clear(); PreviewPanel::updateLayoutMenu(dd->m_layoutMenu); });

    // previewPanel, load settings
    dd->loadPersistentSettings();

    return true;
}

void PreviewPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag PreviewPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

////////////////////////////////////////////////////////////////////////////////

PreviewPluginPrivate::PreviewPluginPrivate()
{

}

void PreviewPluginPrivate::loadPersistentSettings()
{
    QSettings *s = Core::ICore::settings();
    Core::Id id = Core::Id::fromSetting(s->value(QLatin1String(Constants::PREVIEW_LAYOUT)));

    if (!id.isValid())
        id = Core::Id(Constants::PREVIEW_LAYOUT_4X2);

    PreviewPanel::loadLayout(id);

}

} // namespace Internal

} // namespace Preview

