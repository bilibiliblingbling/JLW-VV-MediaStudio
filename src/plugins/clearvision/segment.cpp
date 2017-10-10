#include <QUuid>

#include <utils/communicator/udocaobjecthelper.h>

#include "constants.h"
#include "segment.h"


namespace ClearVision {
namespace Internal {

const char SEGMENT_CAMERAID[]               = "CameraID";
const char SEGMENT_CAMERANAME[]             = "CameraName";
const char SEGMENT_RECORDINGTYPE[]          = "RecordingType";
const char SEGMENT_BEGINTIME[]              = "BeginTime";
const char SEGMENT_ENDTIME[]                = "EndTime";

Segment::Segment(Core::Id mediaSourceId, const UDOCA::Object &obj)
    : ProjectExplorer::Segment(Core::Id::fromString(QUuid::createUuid().toString()))
{
    setMediaSourceId(mediaSourceId);
    setCameraId(Core::Id::fromString(Utils::safeStringProperty(obj, SEGMENT_CAMERAID)));
    setRecordingType(Utils::safeUInt32Property(obj, SEGMENT_RECORDINGTYPE));
    setBeginTime(Utils::safeGMTDateTimeProperty(obj, SEGMENT_BEGINTIME));
    setEndTime(Utils::safeGMTDateTimeProperty(obj, SEGMENT_ENDTIME));

    m_serverId = Utils::safeUStringProperty(obj, Constants::CLEARVISION_SERVERID);
}

Segment::Segment(const Segment &that)
    : ProjectExplorer::Segment(that)
    , m_cameraName(that.m_cameraName)
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

QString Segment::mimeType() const
{
    return QLatin1String(Constants::CLEARVISION_SEGMENT_MIMETYPE);
}

UDOCA::String Segment::serverId() const
{
    return m_serverId;
}

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
} // namespace ClearVision
