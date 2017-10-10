#include <QHBoxLayout>

#include "monitorpanel.h"
#include "fileplayviewwidget.h"
#include "sequenceplayviewwidget.h"

#include "videocontrlwidget.h"

namespace Monitor {
namespace Internal {


static MonitorPanel *s_monitorPanel = 0;

MonitorPanel::MonitorPanel(QWidget *parent)
    : QWidget(parent)
    , m_filePlayViewWidget(new FilePlayViewWidget())
    , m_sequenceplayViewWidget(new SequencePlayViewWidget())
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->setSpacing(4);

    mainLayout->addWidget(m_filePlayViewWidget);
    mainLayout->addWidget(m_sequenceplayViewWidget);

#if 1 // for test
    VideoContrlWidget *ctrl = new VideoContrlWidget;
    ctrl->show();
#endif

    s_monitorPanel = this;
}

MonitorPanel::~MonitorPanel()
{
    s_monitorPanel = 0;
}

MonitorPanel *MonitorPanel::instance()
{
    return s_monitorPanel;
}

} // namespace Internal
} // namespace Monitor
