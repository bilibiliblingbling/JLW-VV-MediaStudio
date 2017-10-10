#include "constants.h"
#include "iqueryroutelistcallback.h"
#include "mediasource.h"
#include "mediasourcetimer.h"
#include "project.h"
#include "sessionmanager.h"


namespace ProjectExplorer {

/*!
    媒体源, 包括类型,


 */

namespace {

const char MEDIASOURCE_CLASSID_PREFIX[] = "ProjectExplorer.MediaSource.ClassId.";

} // namespace anonymous

MediaSource::MediaSource(Core::Id classId)
    : m_classId(classId)
    , m_enabled(false)
{
    // TODO: 重新考虑树节点状态方式, 何时检查连接状态等
    connect(Internal::MediaSourceTimer::instance(), &Internal::MediaSourceTimer::checkStatus,
            this, &MediaSource::checkStatus);
}

MediaSource::MediaSource(Project *pro, Core::Id id, Core::Id classId)
    : MetaData(pro, id)
    , m_classId(classId)
    , m_enabled(true)
{
    // seems unused
}

MediaSource::~MediaSource()
{
    if (Internal::MediaSourceTimer::instance())
        disconnect(Internal::MediaSourceTimer::instance(), 0, this, 0);
}

Core::Id MediaSource::classId() const
{
    return m_classId;
}

/*!
 * \brief MediaSource::className
 * \return 用于展示的媒体源类别名称
 */
QString MediaSource::className() const
{
    return m_classId.suffixAfter(MEDIASOURCE_CLASSID_PREFIX);
}

bool MediaSource::enabled() const
{
    return m_enabled;
}

void MediaSource::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

QString MediaSource::name() const
{
    return m_name;
}

void MediaSource::setName(const QString &name)
{
    m_name = name;
}

QString MediaSource::description() const
{
    return m_description;
}

void MediaSource::setDescription(const QString &description)
{
    m_description = description;
}

void MediaSource::queryRouteList(Segment *segment, IQueryRouteListCallback *callback)
{
    if (Project *project = SessionManager::instance()->startupProject()) {
        if (MediaSource *mediasource = project->mediaSource(segment->mediaSourceId())) {
            mediasource->queryRouteListImpl(segment, callback);
            return;
        } else {
            // mediasource not found
        }
    } else {
        // active project not found
    }

    callback->onQueryRouteListFailed(segment);
}

Core::Id MediaSource::standardClassId(Core::Id id)
{
    return id.withPrefix(MEDIASOURCE_CLASSID_PREFIX);
}

Core::Id MediaSource::standardClassIdPrefix()
{
    return Core::Id(MEDIASOURCE_CLASSID_PREFIX);
}

bool MediaSource::fromMap(const QVariantMap &map)
{
    if (!MetaData::fromMap(map))
        return false;

    if (m_classId != Core::Id::fromSetting(map.value(QLatin1String(Constants::MEDIASOURCE_CLASSID))))
        return false;

    m_enabled = map.value(QLatin1String(Constants::MEDIASOURCE_ENABLED)).toBool();
    m_name = map.value(QLatin1String(Constants::MEDIASOURCE_NAME)).toString();
    m_description = map.value(QLatin1String(Constants::MEDIASOURCE_DESCRIPTION)).toString();

    return true;
}

QVariantMap MediaSource::toMap() const
{
    QVariantMap map(MetaData::toMap());

    map.insert(QLatin1String(Constants::MEDIASOURCE_CLASSID), m_classId.toSetting());
    map.insert(QLatin1String(Constants::MEDIASOURCE_ENABLED), m_enabled);
    map.insert(QLatin1String(Constants::MEDIASOURCE_NAME), m_name);
    map.insert(QLatin1String(Constants::MEDIASOURCE_DESCRIPTION), m_description);

    return map;
}

void MediaSource::queryRouteListImpl(Segment *segment, IQueryRouteListCallback *callback)
{
    // not support
    callback->onQueryRouteListFailed(segment);
}

} // namespace ProjectExplorer
