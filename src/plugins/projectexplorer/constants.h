#ifndef PROJECT_EXPLORER_CONSTANTS_H
#define PROJECT_EXPLORER_CONSTANTS_H

#include <QtGlobal>

namespace ProjectExplorer {
namespace Constants {

// MetaData
const char METADATA_ID[]                        = "ProjectExplorer.MetaData.Id";

// Project
const char PROJECT_NAME[]                       = "ProjectExplorer.Project.Name";

// MediaSource
const char MEDIASOURCE_CLASSID[]                = "ProjectExplorer.MediaSource.ClassId";

// To be compatible with .mstproj
const char MEDIASOURCE_NAME[]                   = "Name";
const char MEDIASOURCE_DESCRIPTION[]            = "Description";
const char MEDIASOURCE_ENABLED[]                = "Enabled";

// Segment
const char SEGMENT_MEDIASOURCEID[]              = "ProjectExplorer.Segment.MediaSourceId";
const char SEGMENT_CAMERAID[]                   = "ProjectExplorer.Segment.CameraId";
const char SEGMENT_RECORDINGTYPE[]              = "ProjectExplorer.Segment.RecordingType";
const char SEGMENT_BEGINTIME[]                  = "ProjectExplorer.Segment.BeginTime";
const char SEGMENT_ENDTIME[]                    = "ProjectExplorer.Segment.EndTime";

//
const char FASTPREVIEW_ID_PREFIX[]              = "ProjectExplorer.FastPreview";


const char PROJECT_MEDIASOURCE_AGENT[]          = "ProjectExplorer.Project.MediaSource.Agent";

// context
const char C_PROJECT[]                          = "ProjectExplorer.Project";
const char C_PROJECTTREEVIEW[]                  = "ProjectExplorer.ProjectTreeView";

// Menus
const char M_ROJECT_RECENTPROJECTS[]            = "ProjectExplorer.Menu.Project.RecentProjects";

// Actions
const char PROJECT_NEW[]                        = "ProjectExplorer.Project.New";
const char PROJECT_OPEN[]                       = "ProjectExplorer.Project.Open";

// mime type
const char PROJECT_MIMETYPE[]                   = "application/x-mediastudio-project";
const char METADATE_MIMETYPE[]                  = "application/x-mediastudio-metadata";
const char SEGMENT_MIMETYPE[]                   = "application/x-mediastudio-segment";
const char CAMERA_MIMETYPE[]                    = "application/x-mediastudio-camera";


} // namespace Constants
} // namespace ProjectExplorer

#endif // PROJECT_EXPLORER_CONSTANTS_H
