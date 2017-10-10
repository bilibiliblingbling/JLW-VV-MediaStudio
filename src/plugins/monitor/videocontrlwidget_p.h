#ifndef VIDEO_CONTRL_WIDGET_P_H
#define VIDEO_CONTRL_WIDGET_P_H

#include <resource/resource.h>
#include <utils/skinbutton.h>

#include <QCheckBox>
#include <QSlider>
#include <QComboBox>

namespace Monitor {
namespace Internal {

class VideoContrlWidgetPrivate
{
public:
    Utils::SkinButton *m_vBtnPlay;
    Utils::SkinButton *m_vBtnStop;
    Utils::SkinButton *m_vBtnPrev;
    Utils::SkinButton *m_vBtnNext;
    Utils::SkinButton *m_vBtnTimeIn;
    Utils::SkinButton *m_vBtnTimeOut;
    Utils::SkinButton *m_vBtnClose;
    Utils::SkinButton *m_vBtnSpeed;

    //QComboBox *m_vSpdCombox;
    QCheckBox *m_vCycleCheckBox;
    QSlider *m_spdSlider;
};

} // namespace Inernal
} // namespace Monitor

#endif // VIDEO_CONTRL_WIDGET_P_H
