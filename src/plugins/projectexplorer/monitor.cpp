#include "constants.h"
#include "monitor.h"
#include "project.h"


namespace ProjectExplorer {
namespace Internal {

class MonitorPrivate
{
public:
    Seg *m_seg;     //!< 不负责释放, 统一由 Project 负责
    Monitor::MonitorType m_type;
};

} // namespace Internal

/*!
    监视器,
 */

Monitor::Monitor(Project *pro, Core::Id id, MonitorType type)
    : MetaData(pro, id)
    , d(new Internal::MonitorPrivate())
{
    d->m_seg = 0;
    d->m_type = type;
}

Monitor::~Monitor()
{
    delete d;
}

Project *Monitor::project() const
{
    return static_cast<Project *>(parent());
}

bool Monitor::fromMap(const QVariantMap &map)
{
    if (!MetaData::fromMap(map))
        return false;

    // TODO

    return true;
}

QVariantMap Monitor::toMap() const
{
    QVariantMap map(MetaData::toMap());

    // TODO

    return map;
}

} // namespace ProjectExplorer
