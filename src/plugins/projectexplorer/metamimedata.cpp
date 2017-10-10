#include "metadata.h"
#include "metamimedata.h"

namespace ProjectExplorer {
namespace Internal {
class MetaMimeDataPrivate
{
public:
    QHash<Core::Id, MetaData *> m_metadatas;
};
} // namespace Internal

MetaMimeData::MetaMimeData()
    : d(new Internal::MetaMimeDataPrivate())
{
}

MetaMimeData::~MetaMimeData()
{
    delete d;
}

bool MetaMimeData::hasFormat(const QString &mimetype) const
{
    foreach (auto metadata, d->m_metadatas) {
        if (metadata->mimeType() == mimetype)
            return true;
    }
    return false;
}

void MetaMimeData::addMetaData(MetaData *metadata)
{
    if (metadata && metadata->isValid()
        && !d->m_metadatas.contains(metadata->id())) {
        d->m_metadatas.insert(metadata->id(), metadata);
    }
}

/*!
 * \brief MetaMimeData::metaDatas
 * \param mimetype
 * \return all MetaDatas if \p mimetype is empty, return MetaDatas with type \p mimetype otherwise
 */
QList<MetaData *> MetaMimeData::metaDatas(const QString &mimetype) const
{
    if (mimetype.isEmpty())
        return d->m_metadatas.values();

    QList<MetaData *> metadatas;
    foreach (auto md, d->m_metadatas) {
        if (md->mimeType() == mimetype)
            metadatas.push_back(md);
    }
    return metadatas;
}

} // namespace ProjectExplorer
