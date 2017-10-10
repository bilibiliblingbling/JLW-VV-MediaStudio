#ifndef RESOURCE_CONSTANTS_H
#define RESOURCE_CONSTANTS_H

#include "resource_global.h"

namespace Resource {
namespace Constants {

// GroupIcons: group_icons.bmp
const int IDX_GRP_WHITE                     = 0;
const int IDX_GRP_BLUE                      = 1;
const int IDX_GRP_RED                       = 2;
const int IDX_GRP_GREEN                     = 3;
const int IDX_GRP_YELLOW                    = 4;
const int IDX_GRP_PEONY                     = 5;
const int IDX_GRP_CYAN                      = 6;
const int IDX_GRP_ORANGE                    = 7;
const int IDX_GRP_GRASS                     = 8;


// PaneIcons: paneicons.bmp
const int IDX_PANE_COPY                     = 0;
const int IDX_PANE_PASTE                    = 1;


// Slider: slider.bmp
const int IDX_SLIDER_CONVEX                 = 0;
const int IDX_SLIDER_CONCAVE                = 1;


// SmallIcons: smallicons.bmp
const int IDX_HOME                          = 0;
const int IDX_CATEGORY                      = 1;

const int IDX_LOGIN_                        = (IDX_HOME+2);
const int IDX_LOGGED_IN                     = (IDX_LOGIN_+0);
const int IDX_LOGGING_IN_RIGHT              = (IDX_LOGIN_+1);
const int IDX_LOGGING_IN_LEFT               = (IDX_LOGIN_+2);
const int IDX_DISABLED                      = (IDX_LOGIN_+3);
const int IDX_OFFLINE                       = (IDX_LOGIN_+4);

const int IDX_DOMAIN                        = (IDX_HOME+8);
const int IDX_MONITOR                       = (IDX_HOME+7);

const int IDX_MONITOR_CONNECTED             = (IDX_MONITOR+1);
const int IDX_MONITOR_CONNECTING_RIGHT      = (IDX_MONITOR+2);
const int IDX_MONITOR_CONNECTING_LEFT       = (IDX_MONITOR+3);
const int IDX_MONITOR_DISABLED              = (IDX_MONITOR+4);
const int IDX_MONITOR_OFFLINE               = (IDX_MONITOR+5);

const int IDX_D8_                           = (IDX_HOME+13);
const int IDX_D8_CONNECTED                  = (IDX_D8_+0);
const int IDX_D8_CONNECTING_RIGHT           = (IDX_D8_+1);
const int IDX_D8_CONNECTING_LEFT            = (IDX_D8_+2);
const int IDX_D8_DISABLED                   = (IDX_D8_+3);
const int IDX_D8_OFFLINE                    = (IDX_D8_+4);

const int IDX_DEV_                          = (IDX_HOME+18);
const int IDX_ENCODER                       = (IDX_DEV_+0);
const int IDX_DECODER                       = (IDX_DEV_+1);
const int IDX_CODEC                         = (IDX_DEV_+2);
const int IDX_CAMERA                        = (IDX_DEV_+3);
const int IDX_NETWORK_CAMERA                = (IDX_DEV_+4);

/// \todo
const int IDX_SEG                           = (IDX_HOME+23);

const int IDX_FOLDER                        = (IDX_HOME+31);

const int IDX_MESSAGE                       = (IDX_HOME+39);
const int IDX_INFOMATION                    = (IDX_MESSAGE+0);
const int IDX_WARNING                       = (IDX_MESSAGE+1);
const int IDX_ERROR                         = (IDX_MESSAGE+2);


// Toolbar: toolbar.bmp
const int IDX_TB_NEW                        = 0;
const int IDX_TB_OPEN                       = 1;
const int IDX_TB_SAVE                       = 2;
const int IDX_TB_CUT                        = 3;
const int IDX_TB_COPY                       = 4;
const int IDX_TB_PASTE                      = 5;
const int IDX_TB_PRINT                      = 6;
const int IDX_TB_HELP                       = 7;


// record_type.bmp
const int IDX_RECORDTYPE_SCHEDULE           = 0;
const int IDX_RECORDTYPE_MANUAL             = 1;
const int IDX_RECORDTYPE_ALARM              = 2;

} // namespace Constants
} // namespace Resource


#endif // RESOURCE_CONSTANTS_H
