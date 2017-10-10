#include "project.h"
#include "seqseg.h"
#include "sequence.h"


namespace ProjectExplorer {

/*!
    时间线, 媒体序列,
 */

namespace Internal {

class SequencePrivate
{
public:
    Sequence::DisplayMode m_mode;   // 显示模式
    quint32 m_pointIn;              // 入点, 单位: ms
    quint32 m_pointOut;             // 出点, 单位: ms
    bool m_playInOut;               // 播放 入点-出点
    bool m_active;                  // 是否默认显示在时间线区域
    bool m_autoAbsorb;              // 自动吸附
    bool m_autoLoop;                // 循环播放

    QString m_name;                 // 显示名称
    QList<SeqSeg *> m_seqSegs;
};

} // namespace Internal

Sequence::Sequence(Project *pro, Core::Id id)
    : MetaData(pro, id)
    , d(new Internal::SequencePrivate())
{
}

Sequence::~Sequence()
{
    delete d;
}

bool Sequence::fromMap(const QVariantMap &map)
{
    if (!MetaData::fromMap(map))
        return false;

    // TODO

    return true;
}

QVariantMap Sequence::toMap() const
{
    QVariantMap map(MetaData::toMap());

    // TODO

    return map;
}

} // namespace ProjectExplorer
