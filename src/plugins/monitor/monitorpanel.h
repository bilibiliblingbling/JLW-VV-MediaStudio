#ifndef MONITOR_PANEL_H
#define MONITOR_PANEL_H

#include <QWidget>

#include "fileplayviewwidget.h"
#include "sequenceplayviewwidget.h"


namespace Monitor {
namespace Internal {

class MonitorPanel : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorPanel(QWidget *parent = 0);
    ~MonitorPanel();

    static MonitorPanel *instance();

private:
    FilePlayViewWidget *m_filePlayViewWidget;
    SequencePlayViewWidget *m_sequenceplayViewWidget;
};

} // namespace Internal
} // namespace Monitor

#endif // MONITOR_PANEL_H
