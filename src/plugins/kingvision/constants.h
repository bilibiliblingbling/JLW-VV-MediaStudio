#ifndef KINGVISION_CONSTANTS_H
#define KINGVISION_CONSTANTS_H

#include <QtGlobal>

namespace KingVision {
namespace Constants {


const char MEDIASOURCE_KINGVISION_ROLE[]        = "MediaStudioLocalStoragePlugin";

// MediaSource Info
const char MEDIASOURCE_CLASSID[]                = "aaf68091-4693-471d-a629-c04efae137b1";
const char MEDIASOURCE_CLASSNAME[]              = QT_TRANSLATE_NOOP("KingVision", "KingVision Storage Server Express");

const char MEDIASOURCE_ARGUMENTS[]              = "Arguments";
const char MEDIASOURCE_STORAGEPROXYADDR[]       = "StorageProxyAddr";
const char MEDIASOURCE_STORAGEPROXYPORT[]       = "StorageProxyPort";

const int  MEDIASOURCE_STORAGEPROXYPORT_DEFAULT = 8772;

//
const char KINGVISION_STORAGESERVEREXPRESS[]    = "StorageServerExpress";


////////////////////////////////////////////////////////////////////////////////
// devices
const char KINGVISION_DEVICE_ID[]                   = "ID";
const char KINGVISION_DEVICE_NAME[]                 = "Name";

// codec property
const char KINGVISION_CODEC_DESCRIPTION[]           = "Description";
const char KINGVISION_CODEC_MODELID[]               = "ModelID";
const char KINGVISION_CODEC_CONNECTIONMODE[]        = "ConnectionMode";
const char KINGVISION_CODEC_CONNECTIONARGUMENTS[]   = "ConnectionArguments";
const char KINGVISION_CODEC_STATUS[]                = "Status";

// camera property
const char KINGVISION_CAMERA_DEVICEID[]             = "DeviceID";
const char KINGVISION_CAMERA_VIDEOPORT[]            = "VideoPort";
const char KINGVISION_CAMERA_STORAGESPACE[]         = "StorageSpace";
const char KINGVISION_CAMERA_MAXSTORAGESPACE[]      = "MaxStorageSpace";
const char KINGVISION_CAMERA_CYCLICCOVERING[]       = "CyclicCovering";
const char KINGVISION_CAMERA_RECORDSTATUS[]         = "RecordStatus";
const char KINGVISION_CAMERA_SCHEDULELIST[]         = "ScheduleList";

////////////////////////////////////////////////////////////////////////////////
// trees
// device tree
const char KINGVISION_DEVICE_VIRTUALROOTNODE[]      = "KingVision.Device.VirtualRootNode";
const char KINGVISION_DEVICE_ROOTNODE[]             = "KingVision.Device.RootNode";

// camera tree
const char KINGVISION_CAMERA_VIRTUALROOT_NODE[]     = "KingVision.Camera.VirtualRootNode";
const char KINGVISION_CAMERA_ROOTNODE[]             = "KingVision.Camera.RootNode";

// query result tree
const char KINGVISION_SEGMENT_VIRTUALROOTNODE[]     = "KingVision.Segment.VirtualRootNode";
const char KINGVISION_SEGMENT_ROOTNODE[]            = "KingVision.Segment.RootNode";


////////////////////////////////////////////////////////////////////////////////
// menus
const char M_KINGVISION_DEVICETREE[]                = "KingVision.Menu.DeviceTree";
const char M_KINGVISION_DEVICE[]                    = "KingVision.Menu.Device";
const char M_KINGVISION_DEVICE_CAMERA[]             = "KingVision.Menu.Device.Camera";

const char M_KINGVISION_SEGMENTTREE[]               = "KingVision.Menu.SegmentTree";
const char M_KINGVISION_SEGMENT_CAMERA[]            = "KingVision.Menu.Segment.Camera";
const char M_KINGVISION_SEGMENT[]                   = "KingVision.Menu.Segment";

// actions group
const char G_KINGVISION_RECORDING[]                 = "KingVision.Group.Recording";
const char G_KINGVISION_NEW[]                       = "KingVision.Group.New";
const char G_KINGVISION_REMOVE[]                    = "KingVision.Group.Remove";
const char G_KINGVISION_PROPERTY[]                  = "KingVision.Group.Property";
const char G_KINGVISION_QUERY[]                     = "KingVision.Group.Query";

// actions
const char KINGVISION_ALLSTARTRECORDING[]           = "KingVision.AllStartRecording";
const char KINGVISION_ALLSTOPRECORDING[]            = "KingVision.AllStopRecording";
const char KINGVISION_NEWDEVICE[]                   = "KingVision.NewDevice";
const char KINGVISION_REMOVEDEVICE[]                = "KingVision.RemoveDevice";
const char KINGVISION_RROPERTY[]                    = "KingVision.Property";
const char KINGVISION_QUERYRECORDINGS[]             = "KingVision.QueryRecordings";
const char KINGVISION_STARTRECORDING[]              = "KingVision.StartRecording";
const char KINGVISION_STOPRECORDING[]               = "KingVision.StopRecording";

// query recordings

////////////////////////////////////////////////////////////////////////////////
// mimetypes
// live stream
const char KINGVISION_CAMERA_MIMETYPE[]             = "application/x-mediastudio-kingvision-camera";
// playback stream
const char KINGVISION_SEGMENT_MIMETYPE[]            = "application/x-mediastudio-kingvision-segment";

} // namespace Constants
} // namespace KingVision

#endif // KINGVISION_CONSTANTS_H
