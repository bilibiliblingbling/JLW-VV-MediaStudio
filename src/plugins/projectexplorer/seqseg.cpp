#include "seqseg.h"

namespace ProjectExplorer {
namespace Internal {

class SeqSegPrivate
{
public:
    Seg *m_seg;
    quint32 m_pos;          //!< 在 sequence 中的位置, 以时间记
    quint32 m_retractIn;    //!< 入点前移
    quint32 m_retractOut;   //!< 出点后移
};

} // namespace Internal


SeqSeg::SeqSeg()
    : d(new Internal::SeqSegPrivate())
{
    d->m_seg = 0;
    d->m_pos = 0;
    d->m_retractIn = d->m_retractOut = 0;
}

SeqSeg::~SeqSeg()
{
    delete d;
}

bool SeqSeg::fromMap(const QVariantMap &map)
{
    // TODO

    return true;
}

QVariantMap SeqSeg::toMap() const
{
    QVariantMap map;

    // TODO

    return map;
}




} // namespace ProjectExplorer
