#ifndef MEDIA_SOURCE_CONSTANTS_H
#define MEDIA_SOURCE_CONSTANTS_H

#include "mediasource_global.h"

namespace MediaSource {
namespace Constants {

// contexts
const char C_MEDIASOURCE_TREEVIEW[]             = "MediaSource.MediaSourceTreeView";
const char C_MEDIASOURCE_BROWSEPANEWIDGET[]     = "MediaSource.BrowsePaneWidget";
const char C_MEDIASOURCE_DEVICETREEVIEW[]       = "MediaSource.DeviceTreeView";

// MediaSourceTree
const char MEDIASOURCETREE_VIRTUALROOTNODE[]    = "MediaSource.MediaSourceTree.VirtualRootNode";
const char MEDIASOURCETREE_ROOTNODE[]           = "MediaSource.MediaSourceTree.VirtualRootNode";

// segment tree
const char SEGMENT_VIRTUALROOTNODE[]            = "MediaSource.Segment.VirtualRootNode";
const char SEGMENT_ROOTNODE[]                   = "MediaSource.Segment.RootNode";

// BrowsePane
const char MEDIASOURCE_DEVICE[]                 = "MediaSource.Device";
const char MEDIASOURCE_QUERY[]                  = "MediaSource.Query";

////////////////////////////////////////////////////////////////////////////////
// media source related menus and actions
// media source tree
const char M_MEDIASOURCE_MEDIASOURCETREE[]      = "MediaSource.Menu.MediaSource.MediaSourceTree";
const char M_MEDIASOURCE_MEDIASOURCECLASS[]     = "MediaSource.Menu.MediaSource.MediaSourceClass";
const char M_MEDIASOURCE_MEDIASOURCE[]          = "MediaSource.Menu.MediaSource.MediaSource";

// group
const char G_MEDIASOURCE_NEW[]                  = "MediaSource.Group.MediaSource.New";

const char G_MEDIASOURCE_BROWSE[]               = "MediaSource.Group.MediaSource.Browse";
const char G_MEDIASOURCE_CONTROL[]              = "MediaSource.Group.MediaSource.Control";
const char G_MEDIASOURCE_MODIFY[]               = "MediaSource.Group.MediaSource.Modify";
const char G_MEDIASOURCE_PROPERTY[]             = "MediaSource.Group.MediaSource.Property";

// actions
const char MEDIASOURCE_NEW[]                    = "MediaSource.MediaSource.New";

const char MEDIASOURCE_BROWSE[]                 = "MediaSource.MediaSource.Browse";
const char MEDIASOURCE_ENABLEORCONNECT[]        = "MediaSource.MediaSource.EnableOrConnect";
const char MEDIASOURCE_DISABLE[]                = "MediaSource.MediaSource.Disable";
const char MEDIASOURCE_RENAME[]                 = "MediaSource.MediaSource.Rename";
const char MEDIASOURCE_REMOVE[]                 = "MediaSource.MediaSource.Remove";
const char MEDIASOURCE_PROPERTY[]               = "MediaSource.MediaSource.Property";

////////////////////////////////////////////////////////////////////////////////
// segment related menus and actions
// segment tree Menu
const char M_SEGMENT_SEGMENTROOT[]              = "MediaSource.Menu.Segment.SegmentRoot";
const char M_SEGMENT_CAMERA[]                   = "MediaSource.Menu.Segment.Camera";
const char M_SEGMENT_SEGEMENT[]                 = "MediaSource.Menu.Segment.Segment";
const char M_SEGMENT_FASTPREVIEW[]              = "MediaSource.Menu.Segment.FastPreview";

// group
const char G_SEGMENT_CLEAR[]                    = "MediaSource.Group.Segment.Clear";
const char G_SEGMENT_REMOVE[]                   = "MediaSource.Group.Segment.Remove";
const char G_SEGMENT_PLAY[]                     = "MediaSource.Group.Segment.Play";
const char G_SEGMENT_ADDTO[]                    = "MediaSource.Group.Segment.AddTo";

const char G_SEGMENT_PREVIEW[]                  = "MediaSource.Group.Segment.Preview";

// segment tree action
const char SEGMENT_CLEAR[]                      = "MediaSource.Segment.Clear";
const char SEGMENT_REMOVE[]                     = "MediaSource.Segment.Remove";
const char SEGMENT_PLAYINMONITOR[]              = "MediaSource.Segment.PlayInMonitor";
const char SEGMENT_ADDTOREPOSITORY[]            = "MediaSource.Segment.AddToRepository";
const char SEGMENT_ADDTOSEQUENCE[]              = "MediaSource.Segment.AddToSequence";

////////////////////////////////////////////////////////////////////////////////
// query condition
// query
const char MEDIASOURCE_QUERY_DISPLAYNAME[]      = "MediaSource.Query.DisplayName";
const char MEDIASOURCE_QUERY_ID[]               = "MediaSource.Query.Id";
const char MEDIASOURCE_QUERY_PRIORITY[]         = "MediaSource.Query.Priority";
const char MEDIASOURCE_QUERY_TIMEBEGIN[]        = "MediaSource.Query.TimeBegin";
const char MEDIASOURCE_QUERY_TIMEEND[]          = "MediaSource.Query.TimeEnd";

// query time range id
const char MEDIASOURCE_QUERY_CUSTOMIZED[]       = "MediaSource.Query.Customized";
const char MEDIASOURCE_QUERY_TODAY[]            = "MediaSource.Query.Today";
const char MEDIASOURCE_QUERY_THISWEEK[]         = "MediaSource.Query.ThisWeek";
const char MEDIASOURCE_QUERY_THISMONTH[]        = "MediaSource.Query.ThisMonth";
const char MEDIASOURCE_QUERY_1DAY[]             = "MediaSource.Query.1Day";
const char MEDIASOURCE_QUERY_3DAYS[]            = "MediaSource.Query.3Days";
const char MEDIASOURCE_QUERY_7DAYS[]            = "MediaSource.Query.7Days";
const char MEDIASOURCE_QUERY_15DAYS[]           = "MediaSource.Query.15Days";
const char MEDIASOURCE_QUERY_30DAYS[]           = "MediaSource.Query.30Days";
const char MEDIASOURCE_QUERY_3MONTHS[]          = "MediaSource.Query.3Months";
const char MEDIASOURCE_QUERY_6MONTHS[]          = "MediaSource.Query.6Months";
const char MEDIASOURCE_QUERY_1YEAR[]            = "MediaSource.Query.1Year";

// query time range name
const char TR_TIMERANGE_CUSTOMIZED[]            = QT_TRANSLATE_NOOP("MediaSource", "Customized");
const char TR_TIMERANGE_TODAY[]                 = QT_TRANSLATE_NOOP("MediaSource", "Today");
const char TR_TIMERANGE_THISWEEK[]              = QT_TRANSLATE_NOOP("MediaSource", "This Week");
const char TR_TIMERANGE_THISMONTH[]             = QT_TRANSLATE_NOOP("MediaSource", "This Month");
const char TR_TIMERANGE_1DAY[]                  = QT_TRANSLATE_NOOP("MediaSource", "1 Day");
const char TR_TIMERANGE_3DAYS[]                 = QT_TRANSLATE_NOOP("MediaSource", "3 Days");
const char TR_TIMERANGE_7DAYS[]                 = QT_TRANSLATE_NOOP("MediaSource", "7 Days");
const char TR_TIMERANGE_15DAYS[]                = QT_TRANSLATE_NOOP("MediaSource", "15 Days");
const char TR_TIMERANGE_30DAYS[]                = QT_TRANSLATE_NOOP("MediaSource", "30 Days");
const char TR_TIMERANGE_3MONTHS[]               = QT_TRANSLATE_NOOP("MediaSource", "3 Months");
const char TR_TIMERANGE_6MONTHS[]               = QT_TRANSLATE_NOOP("MediaSource", "6 Months");
const char TR_TIMERANGE_1YEAR[]                 = QT_TRANSLATE_NOOP("MediaSource", "1 Year");

//

const char MEDIASOURCE_QUERY_TIMERANGEID[]      = "MediaSource.Query.TimeRangeId";

// query recordings
const char MEDIASOURCE_QUERY_CAMERALIST[]       = "MediaSource.Query.CameraList";
const char MEDIASOURCE_QUERY_RECORDTYPE[]       = "MediaSource.Query.RecordType";
const char MEDIASOURCE_QUERY_BGNTIME[]          = "MediaSource.Query.BgnTime";
const char MEDIASOURCE_QUERY_ENDTIME[]          = "MediaSource.Query.EndTime";
const char MEDIASOURCE_QUERY_APPENDRESULTS[]    = "MediaSource.Query.AppendResults";

const int  MEDIASOURCE_RECORDTYPE_SCHEDULE      = (1);
const int  MEDIASOURCE_RECORDTYPE_ALARM         = (2);
const int  MEDIASOURCE_RECORDTYPE_MANUAL        = (4);
const int  MEDIASOURCE_RECORDTYPE_ALL           = (MEDIASOURCE_RECORDTYPE_SCHEDULE|MEDIASOURCE_RECORDTYPE_MANUAL|MEDIASOURCE_RECORDTYPE_ALARM);


// icons
const char ICON_RECORDTYPE_ALARM[]              = ":/mediasource/images/recordtypealarm.png";
const char ICON_RECORDTYPE_MANUAL[]             = ":/mediasource/images/recordtypemanual.png";
const char ICON_RECORDTYPE_SCHEDULE[]           = ":/mediasource/images/recordtypeschedule.png";


// mimetypes
const char SEGMENT_MIMETYPE_PLAYBACK[]          = "application/x-mediastudio-segment-playback";
const char SEGMENT_MIMETYPE_LIVE[]              = "application/x-mediastudio-segment-live";

} // namespace Constants
} // namespace MediaSource

#endif // MEDIA_SOURCE_CONSTANTS_H
