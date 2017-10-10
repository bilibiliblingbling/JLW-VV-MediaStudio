#ifndef PREVIEW_CONSTANTS_H
#define PREVIEW_CONSTANTS_H

#include <QtGlobal>

namespace Preview{
namespace Constants {

// Menus
const char M_PREVIEW_PREVIEWLAYOUTS[]      = "Preview.Menu.PreviewLayouts";

// group for action
const char G_WINDOW_PREVIEWLAYOUTS[]       = "Preview.PreviewLayouts.Group.Layouts";

// Actions 添加在windows菜单下的　previewLayout 的菜单
const char PREVIEW_LAYOUT_4X2[]            = "Preview.Layout.4X2";
const char PREVIEW_LAYOUT_6X2[]            = "Preview.Layout.6X2";
const char PREVIEW_LAYOUT_8X2[]            = "Preview.Layout.8X2";

// videoWidgetSlider styleSheet
const char SLIDERSTYLESHEET[] = "QSlider::groove:horizontal{border: 0.2px solid #165708;background: #8f8f8f;"
                                "height: 2px;border-radius: 1px;padding-left:-1px;padding-right:-1px;}"
                                "QSlider::handle:horizontal{border:1px solid #454343;background:#2af5b9;width:2px;"
                                "margin-top: -2px;margin-bottom: -2px;border-radius: 2px;} "
                                "QSlider::sub-page:horizontal {background:#2af5b9;border: 1px solid #4A708B;height: 2px;border-radius: 2px;}"
                                "QSlider::add-page:horizontal {background: #615f5f;border: 0px solid #2af5b9;height: 1px;border-radius: 2px;}";

const char PREVIEW_LAYOUT[]  = "Preview/Layout";


} // namespace Constants
} // namespace Preview

#endif // PREVIEW_CONSTANTS_H
