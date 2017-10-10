#ifndef FAST_PREVIEW_H
#define FAST_PREVIEW_H

#include "metadata.h"

namespace ProjectExplorer {
class Project;
class Seg;

namespace Internal { class FastPreviewPrivate; }

class PROJECTEXPLORER_EXPORT FastPreview : public MetaData
{
    Q_OBJECT

public:
    FastPreview(Project *pro, Core::Id id);
    ~FastPreview();

    Project *project() const;
    Seg *seg() const;

protected:
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;

private:
    Internal::FastPreviewPrivate *d;
};

} // namespace ProjectExplorer

#endif // FAST_PREVIEW_H
