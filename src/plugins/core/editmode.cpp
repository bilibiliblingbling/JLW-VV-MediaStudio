#include <QVBoxLayout>

#include <utils/styledbar.h>

#include "browsepane.h"
#include "constants.h"
#include "editmode.h"
#include "minisplitter.h"
#include "modemanager.h"
#include "monitorpane.h"
#include "navigationwidget.h"
#include "placeholder.h"
#include "previewpane.h"
#include "timelinepane.h"


namespace Core {
namespace Internal {

EditMode::EditMode()
    : m_splitter(new MiniSplitter())
{
    setObjectName(QLatin1String("Core.EditMode"));
    setDisplayName(tr("Edit"));

    setIcon(QIcon(QLatin1String(Constants::ICON_EDIT_MODE)));
    setPriority(Constants::P_MODE_EDIT);
    setId(Constants::MODE_EDIT);

    /*!
     * +---+--------+---------+
     * |   | browse | monitor |
     * | n | widget | widget  |
     * | a +--------+---------+
     * | v |  preview widget  |
     * | i +------------------+
     * |   | timeline  widget |
     * +---+------------------+
     */

    MiniSplitter *rightTopSplitter = new MiniSplitter;
    rightTopSplitter->insertWidget(0, new PlaceHolder<BrowsePane>(this));
    rightTopSplitter->insertWidget(1, new PlaceHolder<MonitorPane>(this));
    rightTopSplitter->setStretchFactor(0, 0);
    rightTopSplitter->setStretchFactor(1, 1);


    MiniSplitter *rightSplitter = new MiniSplitter;
    rightSplitter->setOrientation(Qt::Vertical);
    rightSplitter->insertWidget(0, rightTopSplitter);
    rightSplitter->insertWidget(1, new PlaceHolder<PreviewPane>(this));
    rightSplitter->insertWidget(2, new PlaceHolder<TimelinePane>(this));
    rightSplitter->setStretchFactor(0, 1);
    rightSplitter->setStretchFactor(1, 1);
    rightSplitter->setStretchFactor(2, 1);

    m_splitter->insertWidget(0, new NavigationWidgetPlaceHolder(this));
    m_splitter->insertWidget(1, rightSplitter);
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);

    connect(ModeManager::instance(), &ModeManager::currentModeChanged,
            this, &EditMode::grapFocus);

    setWidget(m_splitter);
    setContext(Context(Constants::C_EDIT_MODE,
                       Constants::C_NAVIGATION_PANE));
}

EditMode::~EditMode()
{
    delete m_splitter;
}

void EditMode::grapFocus(IMode *mode)
{
    if (mode != this)
        return;

    widget()->setFocus();
}

} // namespace Internal
} // namespace Core
