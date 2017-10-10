#ifndef SEQ_SEG_H
#define SEQ_SEG_H

#include <QVariantMap>

#include "projectexplorer_global.h"

namespace ProjectExplorer {
class Seg;

namespace Internal { class SeqSegPrivate; }

class PROJECTEXPLORER_EXPORT SeqSeg
{
public:
    SeqSeg();
    ~SeqSeg();

public:
    bool fromMap(const QVariantMap &map);
    QVariantMap toMap() const;

private:
    Internal::SeqSegPrivate *d;
};

} // namespace ProjectExplorer

#endif // SEG_SEG_H
