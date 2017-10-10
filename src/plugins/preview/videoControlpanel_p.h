#ifndef VIDEO_CONTROL_PANEL_P
#define VIDEO_CONTROL_PANEL_P

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <qglobal.h>
#include <utils/skinbutton.h>
#include <resource/resource.h>
#include <QStackedWidget>

#include "slider.h"
#include "timeedit.h"

namespace Preview {
namespace Internal {

class VideoControlPanelPrivate
{
public:
    Utils::SkinButton *m_btnPlay;
    Utils::SkinButton *m_btnPause;
    Utils::SkinButton *m_btnStop;
    Utils::SkinButton *m_btnSetTimeIn;
    Utils::SkinButton *m_btnSetTimeOut;
    Utils::SkinButton *m_btnClose;
    Utils::SkinButton *m_btnstatus;

    QSpacerItem *m_spacer;
    //QLabel *m_timeLabel;
    timeEdit *m_timeEdit;

    QHBoxLayout *m_Hlayout;
    QVBoxLayout *m_Vlayout;

    Slider *m_slider;
    QStackedWidget *m_stackedWidget;
};

} //namespace Internal
} //namespace Preview

#endif // VIDEO_CONTROL_PANEL_P

