#include "constants.h"
#include "fastpreview.h"
#include "project.h"

namespace ProjectExplorer {
namespace Internal {

class FastPreviewPrivate
{
public:
    Seg *m_seg;
};

} // namespace Internal

/*!
    快速预览部分,

 */

FastPreview::FastPreview(Project *pro, Core::Id id)
    : MetaData(pro, id)
    , d(new Internal::FastPreviewPrivate())
{
    d->m_seg = 0;
}

FastPreview::~FastPreview()
{
    delete d;
}

Project *FastPreview::project() const
{
    return static_cast<Project *>(parent());
}

Seg *FastPreview::seg() const
{
    return d->m_seg;
}

bool FastPreview::fromMap(const QVariantMap &map)
{
    if (!MetaData::fromMap(map))
        return false;

    // TODO

    return true;
}

QVariantMap FastPreview::toMap() const
{
    QVariantMap map(MetaData::toMap());

    // TODO

    return map;
}

} // namespace ProjectExplorer
