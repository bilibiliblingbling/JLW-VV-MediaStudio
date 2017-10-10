#include <QUuid>

#include <utils/communicator/udocaobjecthelper.h>
#include <utils/tmplog.h>

#include "constants.h"
#include "segment.h"

namespace KingVision {
namespace Internal {

const char SEGMENT_CAMERAID[]               = "CameraID";
const char SEGMENT_RECORDINGTYPE[]          = "RecordingType";
const char SEGMENT_BEGINTIME[]              = "BeginTime";
const char SEGMENT_ENDTIME[]                = "EndTime";
const char SEGMENT_HASPREVIEW[]             = "HasPreview";



Segment::Segment(Core::Id mediaSourceId, const UDOCA::Object &obj)
    : ProjectExplorer::Segment(Core::Id::fromString(QUuid::createUuid().toString()))
{
    setMediaSourceId(mediaSourceId);
    setCameraId(Core::Id::fromString(Utils::safeStringProperty(obj, SEGMENT_CAMERAID)));
    setRecordingType(Utils::safeUInt32Property(obj, SEGMENT_RECORDINGTYPE));
    setBeginTime(Utils::safeGMTDateTimeProperty(obj, SEGMENT_BEGINTIME));
    setEndTime(Utils::safeGMTDateTimeProperty(obj, SEGMENT_ENDTIME));
}

Segment::Segment(const Segment &that)
    : ProjectExplorer::Segment(that)
    , m_hasPreview(that.m_hasPreview)
{
}

Segment *Segment::create(Core::Id srcId, const UDOCA::Object &obj)
{
    Segment *segment = new Segment(srcId, obj);
    if (!segment->isValid()) {
        delete segment;
        segment = 0;
    }
    return segment;
}

/*!
 * \brief Segment::clone
 * \return a deep-copy of this
 */
Segment *Segment::clone() const
{
    Segment *segment = new Segment(*this);
    if (!segment->isValid()) {
        delete segment;
        segment = 0;
    }
    return segment;
}

} // namespace Internal
} // namespace KingVision
