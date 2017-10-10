#include <QSettings>

#include "monitorpane.h"

namespace Core {
namespace Internal {

/*!
 * \brief MonitorPane
 *  Monitor 区域, 精细编辑 以及 最终展示
 */

namespace {

const char PANE_SIZE[]                      = "MonitorPane/Size";
const char PANE_VISIBLE[]                   = "MonitorPane/Visible";
const int  PANE_SIZE_DEFAULT                = 400;

} // namespace anonymous


MonitorPane::MonitorPane(QAction *toggleMonitorAction, QWidget *parent)
    : BaseType(toggleMonitorAction, parent)
{
    setObjectName(QLatin1String("Core.MonitorPane"));
    setOrientation(Qt::Horizontal);
}

MonitorPane::~MonitorPane()
{
}

void MonitorPane::saveSettings(QSettings *settings) const
{
    settings->setValue(QLatin1String(PANE_VISIBLE), isShown());
    settings->setValue(QLatin1String(PANE_SIZE), storedSize());
}

void MonitorPane::restoreSettings(QSettings *settings)
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

} // namespace Internal
} // namespace Core
