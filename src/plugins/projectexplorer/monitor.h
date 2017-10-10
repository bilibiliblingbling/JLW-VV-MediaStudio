#ifndef MONITOR_H
#define MONITOR_H

#include "metadata.h"

namespace ProjectExplorer {
class Project;
class Seg;
class Sequence;

namespace Internal { class MonitorPrivate; }

class PROJECTEXPLORER_EXPORT Monitor : public MetaData
{
    Q_OBJECT

public:
    enum MonitorType {
          MaterialMonitorType       //!< 精细编辑, 片段播放
        , SequenceMonitorType       //!< 时间线预览, 时间线播放
    };

public:
    Monitor(Project *pro, Core::Id id, MonitorType type = MaterialMonitorType);
    ~Monitor();

    Project *project() const;

public:
    MonitorType type() const;

    Seg *seg() const;
    Sequence *sequence() const;

protected:
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;

private:
    Internal::MonitorPrivate *d;
};

} // namespace ProjectExplorer

#endif // MONITOR_H
