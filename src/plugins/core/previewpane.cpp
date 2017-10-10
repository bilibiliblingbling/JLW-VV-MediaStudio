#include <QSettings>

#include "previewpane.h"

namespace Core {
namespace Internal {

/*!
 * \brief PreviewPane
 *  快速预览
 */

namespace {

const char PANE_SIZE[]                      = "PreviewPane/Size";
const char PANE_VISIBLE[]                   = "PreviewPane/Visible";
const int  PANE_SIZE_DEFAULT                = 240;

} // namespace anonymous

PreviewPane::PreviewPane(QAction *togglePreviewAction, QWidget *parent)
    : BaseType(togglePreviewAction, parent)
{
    setObjectName(QLatin1String("Core.PreviewPane"));
    setOrientation(Qt::Vertical);
}

PreviewPane::~PreviewPane()
{
}

void PreviewPane::saveSettings(QSettings *settings) const
{
    settings->setValue(QLatin1String(PANE_VISIBLE), isShown());
    settings->setValue(QLatin1String(PANE_SIZE), storedSize());
    // TODO: should support toggle action in menu
}

void PreviewPane::restoreSettings(QSettings *settings)
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
