#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "metadata.h"

namespace ProjectExplorer {
class Project;
class SeqSeg;

namespace Internal { class SequencePrivate; }


class PROJECTEXPLORER_EXPORT Sequence : public MetaData
{
    Q_OBJECT

public:
    enum DisplayMode {
          NameOnly          = 0     // 仅显示名称
        , FirstFrame        = 1     // 仅显示首帧
        , FirstLastFrame    = 2     // 显示首尾帧
    };

public:
    Sequence(Project *pro, Core::Id id);
    ~Sequence();

protected:
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;

private:
    Internal::SequencePrivate *d;
};

} // namespace ProjectExplorer

#endif // SEQUENCE_H
