#ifndef CLEARVISION_CONSTANTS_H
#define CLEARVISION_CONSTANTS_H

#include <QtGlobal>

namespace ClearVision {
namespace Constants {

const char CLEARVISION_TR_SCOPE[]               = "ClearVision";

const char MEDIASOURCE_CLEARVISION_ROLE[]       = "PlaybackCenter";
const char MEDIASOURCE_CLASSID[]                = "58346495-f564-47f8-be2b-6a41d7b61b58";
const char TR_MEDIASOURCE_CLASSNAME[]           = QT_TRANSLATE_NOOP("ClearVision", "ClearVision Storage Server");

// arguments
const char MEDIASOURCE_ARGUMENTS[]              = "Arguments";
const char MEDIASOURCE_USERNAME[]               = "Username";
const char MEDIASOURCE_PASSWORD[]               = "Password";
const char MEDIASOURCE_SERVERADDR[]             = "ServerAddr";
const char MEDIASOURCE_SERVERPORT[]             = "ServerPort";

const int  MEDIASOURCE_SERVERPORT_DEFAULT       = 8000;

const char CLEARVISION_CENTRALSERVER[]          = "CentralServer";

// clearvision data
const char CLEARVISION_OBJECT_ID[]              = "ID";
const char CLEARVISION_OBJECT_NAME[]            = "Name";
const char CLEARVISION_OBJECT_CLASS[]           = "Class";
const char CLEARVISION_OBJECT_ORGUNITID[]       = "OrganizationalUnitID";
const char CLEARVISION_OBJECT_ORGPATH[]         = "OrganizationalPath";


const char CLEARVISION_OBJECT_ROOTID[]          = "/";

const char CLEARVISION_SERVERID[]               = "ServerID";

// priority
const int  CLEARVISION_UNIT_PRIORITY            = 1;
const int  CLEARVISION_CAMERA_PRIORITY          = 2;
const int  CLEARVISION_DOMAIN_PRIORITY          = 3;


////////////////////////////////////////////////////////////////////////////////
// trees
// camera tree
const char CAMERA_VIRTUALROOTNODE[]             = "ClearVision.Camera.VirtualRootNode";
const char CAMERA_ROOTNODE[]                    = "ClearVision.Camera.RootNode";


const char TR_CAMERAS[]                         = QT_TRANSLATE_NOOP("ClearVision", "Cameras");
const char TR_UNITINFO_REQUESTING[]             = QT_TRANSLATE_NOOP("ClearVision", "Requesting Children...");
const char TR_UNITINFO_FAILED1[]                = QT_TRANSLATE_NOOP("ClearVision", "Request Children Failed: %1");


////////////////////////////////////////////////////////////////////////////////
// mimetypes
// playback mimetype
const char CLEARVISION_SEGMENT_MIMETYPE[]       = "application/x-mediastudio-clearvision-segment";

} // namespace Constants
} // namespace ClearVision

#endif // CLEARVISION_CONSTANTS_H
