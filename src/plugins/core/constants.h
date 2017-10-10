#ifndef CORE_CONSTANTS_H
#define CORE_CONSTANTS_H

#include <QtGlobal>

namespace Core {
namespace Constants {


const char CORE_TR_SCOPE[]              = "Core";

// Modes
const char MODE_WELCOME[]               = "Welcome";
const char MODE_EDIT[]                  = "Edit";
const char MODE_ENJOY[]                 = "Enjoy";

const int  P_MODE_WELCOME               = 100;
const int  P_MODE_EDIT                  = 90;
const int  P_MODE_ENJOY                 = 80;


// MenuBar
const char MENU_BAR[]                   = "MediaStudio.MenuBar";


// Menus
const char M_FILE[]                     = "MediaStudio.Menu.File";
const char M_EDIT[]                     = "MediaStudio.Menu.Edit";
const char M_WINDOW[]                   = "MediaStudio.Menu.Window";
const char M_TOOLS[]                    = "MediaStudio.Menu.Tools";
const char M_HELP[]                     = "MediaStudio.Menu.Help";


// Contexts
const char C_GLOBAL[]                   = "Global Context";
const char C_NAVIGATION_PANE[]          = "Core.NavigationPane";
const char C_WELCOME_MODE[]             = "Core.Welcome";
const char C_EDIT_MODE[]                = "Core.EditMode";
const char C_ENJOY_MDOE[]               = "Core.EnjoyMode";


// Actions
const char EXIT[]                       = "MediaStudio.Exit";
const char OPTIONS[]                    = "MediaStudio.Options";
const char MANUAL[]                     = "MediaStudio.Manual";
const char ABOUT[]                      = "MediaStudio.About";


// Default Groups
const char G_DEFAULT_ONE[]              = "MediaStudio.Group.Default.One";
const char G_DEFAULT_TWO[]              = "MediaStudio.Group.Default.Two";
const char G_DEFAULT_THREE[]            = "MediaStudio.Group.Default.Three";

// MenuBar Groups
const char G_FILE[]                     = "MediaStudio.Group.File";
const char G_EDIT[]                     = "MediaStudio.Group.Edit";
const char G_WINDOW[]                   = "MediaStudio.Group.Window";
const char G_TOOLS[]                    = "MediaStudio.Group.Tools";
const char G_HELP[]                     = "MediaStudio.Group.Help";


// group here, add there


// File Menu Groups
const char G_FILE_PROJECT[]             = "MediaStudio.Group.File.Project";
const char G_FILE_NEW[]                 = "MediaStudio.Group.File.New";
const char G_FILE_OPEN[]                = "MediaStudio.Group.File.Open";
const char G_FILE_SAVE[]                = "MediaStudio.Group.File.Save";
const char G_FILE_CLOSE[]               = "MediaStudio.Group.File.Close";
const char G_FILE_OTHTER[]              = "MediaStudio.Group.File.Other";

// Window Menu Groups
const char G_WINDOW_VIEWS[]             = "MediaStudio.Group.Window.Views";
const char G_WINDOW_LIST[]              = "MediaStudio.Group.Window.List";

// Tools Menu Groups
const char G_TOOLS_OPTIONS[]            = "MediaStudio.Group.Tools.Options";

// Help Menu Groups
const char G_HELP_MANUAL[]              = "MediaStudio.Group.Help.Manual";
const char G_HELP_ABOUT[]               = "MediaStudio.Group.Help.About";



const char TOGGLE_MODE_SELECTPR[]       = "MediaStudio.ToggleModeSelector";
const char TOGGLE_SIDEBAR[]             = "MediaStudio.ToggleSideBar";
const char TOGGLE_FULLSCREEN[]          = "MediaStudio.ToggleFullScreen";
const char TOGGLE_MONITOR[]             = "MediaStudio.ToggleMonitor";
const char TOGGLE_PREVIEW[]             = "MediaStudio.TogglePreview";
const char TOGGLE_TIMELINE[]            = "MediaStudio.ToggleTimeline";

const char TR_SHOW_SIDEBAR[]            = QT_TRANSLATE_NOOP("Core", "Show Sidebar");
const char TR_HIDE_SIDEBAR[]            = QT_TRANSLATE_NOOP("Core", "Hide Sidebar");
const char TR_SHOW_MONITOR[]            = QT_TRANSLATE_NOOP("Core", "Show Monitor");
const char TR_HIDE_MONITOR[]            = QT_TRANSLATE_NOOP("Core", "Hide Monitor");
const char TR_SHOW_PREVIEW[]            = QT_TRANSLATE_NOOP("Core", "Show Preview");
const char TR_HIDE_PREVIEW[]            = QT_TRANSLATE_NOOP("Core", "Hide Preview");
const char TR_SHOW_TIMELINE[]           = QT_TRANSLATE_NOOP("Core", "Show Timeline");
const char TR_HIDE_TIMELINE[]           = QT_TRANSLATE_NOOP("Core", "Hide Timeline");

const char ALL_FILES_FILTER[]           = QT_TRANSLATE_NOOP("Core", "All Files (*)");
const char TR_CLEAR_MENU[]              = QT_TRANSLATE_NOOP("Core", "Clear Menu");
const char TR_CLOSE[]                   = QT_TRANSLATE_NOOP("Core", "Close");


// Icons
const char ICON_MEDIASTUDIO[]           = ":/core/images/mediastudio.ico";
const char ICON_HELP[]                  = ":/core/images/help.png";

const char ICON_EDIT_MODE[]             = ":/core/images/mediastudio.ico";
const char ICON_CLOSE_BUTTON[]          = ":/core/images/closebutton.png";
const char ICON_TOGGLE_SIDEBAR[]        = ":/core/images/sidebaricon.png";


const int  TARGET_ICON_SIZE             = 24;


//
const char SETTINGS_THEME[]             = "Core/MediaStudioTheme";
} // namespace Constants
} // namespace Core

#endif // CORE_CONSTANTS_H
