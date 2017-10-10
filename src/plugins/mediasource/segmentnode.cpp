#include <resource/resource.h>

#include "constants.h"
#include "segmentnode.h"

namespace MediaSource {
namespace Internal {

SegmentNode::SegmentNode(PE::Segment *segment)
    : m_segment(segment)
{
}

QString SegmentNode::displayName() const
{
    QDateTime bgn = m_segment->bgnDateTime();
    QDateTime end = m_segment->endDateTime();

    QString bgnformat, endformat;
    bool diffy = (bgn.date().year() != end.date().year());
    bool diffm = (bgn.date().month() != end.date().month());
    bool diffd = (bgn.date().day() != end.date().day());

    if (diffy) {
        bgnformat = QLatin1String("(yyyy-MM-dd)hh:mm:ss");
        endformat = QLatin1String("hh:mm:ss(yyyy-MM-dd)");
    } else {
        if (bgn.date().year() != QDate::currentDate().year()) {
            bgnformat = QLatin1String("(yyyy-MM-dd)hh:mm:ss");
        } else {
            bgnformat = QLatin1String("(MM-dd)hh:mm:ss");
        }

        endformat = QLatin1String("hh:mm:ss");
        if (diffm || diffd) {
            endformat += QLatin1String("(MM-dd)");
        }
    }

    return (bgn.toString(bgnformat) + QLatin1Char('-') + end.toString(endformat));
}

QIcon SegmentNode::icon() const
{
    QString iconfilepath;

    switch(m_segment->recordingType()) {
    case Constants::MEDIASOURCE_RECORDTYPE_SCHEDULE:
        iconfilepath = QLatin1String(Constants::ICON_RECORDTYPE_SCHEDULE);
        break;
    case Constants::MEDIASOURCE_RECORDTYPE_MANUAL:
        iconfilepath = QLatin1String(Constants::ICON_RECORDTYPE_MANUAL);
        break;
    case Constants::MEDIASOURCE_RECORDTYPE_ALARM:
        iconfilepath = QLatin1String(Constants::ICON_RECORDTYPE_ALARM);
        break;
    default:
        iconfilepath = QLatin1String(Constants::ICON_RECORDTYPE_SCHEDULE);
        break;
    }

    return QIcon(iconfilepath);
}

PE::Segment *SegmentNode::metadata() const
{
    return m_segment;
}

} // namespace Internal
} // namespace MediaSource
