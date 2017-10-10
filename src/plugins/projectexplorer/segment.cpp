#include <core/id.h>

#include "constants.h"
#include "mediasource.h"
#include "project.h"
#include "segment.h"

namespace ProjectExplorer {

const char SEGMENT_INFO_ID_PREFIX[]         = "ProjectExplorer.Segment.Info.";

static Core::Id s_segment_info_prefix(SEGMENT_INFO_ID_PREFIX);

Segment::Segment(Core::Id id)
    : MetaData(0, id)
{
}

Segment::Segment(const Segment &that)
    : MetaData(that)
    , m_mediaSourceId(that.m_mediaSourceId)
    , m_cameraId(that.m_cameraId)
    , m_recordingType(that.m_recordingType)
    , m_bgnTime(that.m_bgnTime)
    , m_endTime(that.m_endTime)
{
}

Segment::~Segment()
{
}

Core::Id Segment::mediaSourceId() const
{
    return m_mediaSourceId;
}

Core::Id Segment::cameraId() const
{
    return m_cameraId;
}

qint32 Segment::recordingType() const
{
    return m_recordingType;
}

QDateTime Segment::bgnDateTime() const
{
    return m_bgnTime;
}

QDateTime Segment::endDateTime() const
{
    return m_endTime;
}

quint64 Segment::bgnTime() const
{
    return m_bgnTime.toMSecsSinceEpoch()/1000;
}

quint64 Segment::endTime() const
{
    return m_endTime.toMSecsSinceEpoch()/1000;
}

/*!
 * \brief Segment::infoId
 * \return \c id that based on cameraId, recordType, beginTime, endTime
 */
Core::Id Segment::infoId() const
{
    return s_segment_info_prefix.withSuffix(cameraId())
            .withSuffix(QString(".%1.%2.%3")
                        .arg(recordingType())
                        .arg(bgnTime())
                        .arg(endTime()));
}

QString Segment::mimeType() const
{
    return QLatin1String(Constants::SEGMENT_MIMETYPE);
}

bool Segment::fromMap(const QVariantMap &map)
{
    if (!MetaData::fromMap(map))
        return false;

    m_mediaSourceId = Core::Id::fromSetting(map.value(QLatin1String(Constants::SEGMENT_MEDIASOURCEID)));
    m_cameraId = Core::Id::fromSetting(map.value(QLatin1String(Constants::SEGMENT_CAMERAID)));
    m_recordingType = map.value(QLatin1String(Constants::SEGMENT_RECORDINGTYPE), 7).toInt();
    m_bgnTime = map.value(QLatin1String(Constants::SEGMENT_BEGINTIME)).toDateTime();
    m_endTime = map.value(QLatin1String(Constants::SEGMENT_ENDTIME)).toDateTime();

    return true;
}

QVariantMap Segment::toMap() const
{
    QVariantMap map(MetaData::toMap());

    map.insert(QLatin1String(Constants::SEGMENT_MEDIASOURCEID), m_mediaSourceId.toSetting());
    map.insert(QLatin1String(Constants::SEGMENT_CAMERAID), m_cameraId.toSetting());
    map.insert(QLatin1String(Constants::SEGMENT_RECORDINGTYPE), m_recordingType);
    map.insert(QLatin1String(Constants::SEGMENT_BEGINTIME), m_bgnTime);
    map.insert(QLatin1String(Constants::SEGMENT_ENDTIME), m_endTime);

    return map;
}

void Segment::setMediaSourceId(Core::Id mediaSourceId)
{
    m_mediaSourceId = mediaSourceId;
}

void Segment::setCameraId(Core::Id cameraId)
{
    m_cameraId = cameraId;
}

void Segment::setRecordingType(qint32 recordingType)
{
    m_recordingType = recordingType;
}

void Segment::setBeginTime(const QDateTime &dt)
{
    m_bgnTime = dt;
}

void Segment::setEndTime(const QDateTime &dt)
{
    m_endTime = dt;
}


const char SEG_OFST_IN[]            = "ProjectExplorer.Project.Seg.OfstIn";
const char SEG_OFST_OUT[]           = "ProjectExplorer.Project.Seg.OfstOut";
const char SEG_NAME[]               = "ProjectExplorer.Project.Seg.Name";


Seg::Seg(Project *pro, Core::Id id)
    : MetaData(pro, id)
    , m_ofstIn(0)
    , m_ofstOut(0)
{
}

Seg::~Seg()
{
}

Project *Seg::project() const
{
    return static_cast<Project *>(parent());
}

QString Seg::name() const
{
    return m_name;
}

void Seg::setName(const QString &name)
{
    m_name = name;
}

quint32 Seg::ofstIn() const
{
    return m_ofstIn;
}

quint32 Seg::ofstOut() const
{
    return m_ofstOut;
}

void Seg::setOfstIn(quint32 in)
{
    m_ofstIn = in;
}

void Seg::setOfstOut(quint32 out)
{
    m_ofstOut = out;
}

bool Seg::fromMap(const QVariantMap &map)
{
    if (!MetaData::fromMap(map))
        return false;

    bool ok = false;

    m_ofstIn = map.value(QLatin1String(SEG_OFST_IN), 0).toUInt(&ok);
    if (!ok)
        m_ofstIn = 0;

    m_ofstOut = map.value(QLatin1String(SEG_OFST_OUT), 0).toUInt(&ok);
    if (!ok)
        m_ofstOut = 0;

    m_name = map.value(QLatin1String(SEG_NAME)).toString();

    return true;
}

QVariantMap Seg::toMap() const
{
    QVariantMap map(MetaData::toMap());

    map.insert(QLatin1String(SEG_NAME), m_name);
    map.insert(QLatin1String(SEG_OFST_IN), m_ofstIn);
    map.insert(QLatin1String(SEG_OFST_OUT), m_ofstOut);

    return map;
}


} // namespace ProjectExplorer
