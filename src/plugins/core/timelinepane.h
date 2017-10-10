#ifndef TIMELINE_PANE_H
#define TIMELINE_PANE_H

#include "pane_p.h"

namespace Core {
namespace Internal {

class TimelinePane : public PaneImpl<TimelinePane>
{
    Q_OBJECT

public:
    explicit TimelinePane(QAction *toggleTimelineAction = 0, QWidget *parent = 0);
    ~TimelinePane();

public:
    void saveSettings(QSettings *settings) const;
    void restoreSettings(QSettings *settings);

public:
    void setShown(bool b);
};

} // namespace Internal
} // namespace Core

#endif // TIMELINE_PANE_H
