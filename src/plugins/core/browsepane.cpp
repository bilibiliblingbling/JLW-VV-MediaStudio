#include <QSettings>

#include "browsepane.h"

namespace Core {
namespace Internal {

/*!
 * \brief BrowsePane
 *  查询录像, 浏览监控点... 显示区域, 不长显示
 */

namespace {

const char PANE_SIZE[]                      = "BrowsePane/Size";
const int  PANE_SIZE_DEFAULT                = 240;

} // namespace anonymous


BrowsePane::BrowsePane(QWidget *parent)
    : BaseType(0, parent)
{
    setObjectName(QLatin1String("Core.BrowsePane"));
    setOrientation(Qt::Horizontal);
}

BrowsePane::~BrowsePane()
{
}

void BrowsePane::saveSettings(QSettings *settings) const
{
    settings->setValue(QLatin1String(PANE_SIZE), storedSize());
}

void BrowsePane::restoreSettings(QSettings *settings)
{
    qint32 size = 0;
    if (settings->contains(QLatin1String(PANE_SIZE))) {
        size = settings->value(QLatin1String(PANE_SIZE), PANE_SIZE_DEFAULT).toInt();
    }
    restoreSize(size? size: PANE_SIZE_DEFAULT);

    // Apply
    if (HolderType::m_current) {
        HolderType::m_current->applyStoredSize(storedSize());
    }
}

bool BrowsePane::isShown() const
{
    return validateWidget();
}

void BrowsePane::setShown(bool shown)
{
    if (HolderType::m_current) {
        HolderType::m_current->setVisible(shown && validateWidget());
    }

    BaseType::setShown(shown);
}

} // namespace Internal
} // namespace Core
