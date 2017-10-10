#ifndef BROWSE_PANE_H
#define BROWSE_PANE_H

#include "core_global.h"

#include "pane_p.h"
#include "placeholder.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class BrowsePane : public PaneImpl<BrowsePane>
{
    Q_OBJECT

public:
    explicit BrowsePane(QWidget *parent = 0);
    ~BrowsePane();

public:
    void saveSettings(QSettings *settings) const;
    void restoreSettings(QSettings *settings);

public:
    bool isShown() const;
    void setShown(bool shown);
};

} // namespace Internal
} // namespace Core

#endif // BROWSE_PANE_H
