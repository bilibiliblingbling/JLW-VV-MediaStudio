#ifndef MONITOR_PANE_H
#define MONITOR_PANE_H

#include "pane_p.h"
#include "placeholder.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class MonitorPane : public PaneImpl<MonitorPane>
{
    Q_OBJECT

public:
    explicit MonitorPane(QAction *toggleMonitorAction = 0, QWidget *parent = 0);
    ~MonitorPane();

public:
    void saveSettings(QSettings *settings) const;
    void restoreSettings(QSettings *settings);
};

} // namespace Internal
} // namespace Core

#endif // MONITOR_PANE_H
