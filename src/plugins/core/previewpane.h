#ifndef PREVIEW_PANE_H
#define PREVIEW_PANE_H

#include "pane_p.h"
#include "placeholder.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class PreviewPane : public PaneImpl<PreviewPane>
{
    Q_OBJECT

public:
    explicit PreviewPane(QAction *togglePreviewAction = 0, QWidget *parent = 0);
    ~PreviewPane();

public:
    void saveSettings(QSettings *settings) const;
    void restoreSettings(QSettings *settings);
};

} // namespace Internal
} // namespace Core

#endif // PREVIEW_PANE_H
