#include <QSettings>

#include "timelinepane.h"

namespace Core {
namespace Internal {

/*!
 * \brief TimeLinePane
 *  时间线
 */

namespace {

const char PANE_SIZE[]                      = "TimelinePane/Size";
const char PANE_VISIBLE[]                   = "TimelinePane/Visible";
const int  PANE_SIZE_DEFAULT                = 240;

} // namespace anonymous


TimelinePane::TimelinePane(QAction *toggleTimelineAction, QWidget *parent)
    : BaseType(toggleTimelineAction, parent)
{
    setObjectName(QLatin1String("Core.TimelinePane"));
    setOrientation(Qt::Vertical);
}

TimelinePane::~TimelinePane()
{
}

void TimelinePane::saveSettings(QSettings *settings) const
{
    settings->setValue(QLatin1String(PANE_VISIBLE), isShown());
    settings->setValue(QLatin1String(PANE_SIZE), storedSize());
}

void TimelinePane::restoreSettings(QSettings *settings)
{
    qint32 size = 0;
    if (settings->contains(QLatin1String(PANE_SIZE))) {
        size = settings->value(QLatin1String(PANE_SIZE), PANE_SIZE_DEFAULT).toInt();
    }
    restoreSize(size? size: PANE_SIZE_DEFAULT);

    setShown(settings->value(QLatin1String(PANE_VISIBLE), true).toBool());

    // Apply
    if (HolderType::m_current) {
        HolderType::m_current->applyStoredSize(storedSize());
    }
}

void TimelinePane::setShown(bool b)
{
    if (HolderType::current())
        HolderType::current()->setVisible(b && validateWidget());

    BaseType::setShown(b);
}

} // namespace Internal
} // namespace Core
