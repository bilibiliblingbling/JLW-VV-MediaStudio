#include <QMimeDatabase>
#include <QSet>

#include "mimedatabase.h"

namespace Utils {

static QList<QMimeType> s_mimetypes;    // mimetypes those used by this program

MimeType MimeDatabase::mimeTypeForName(const QString &mimetype)
{
    QMimeDatabase mdb;
    return mdb.mimeTypeForName(mimetype);
}

MimeType MimeDatabase::mimeTypeForFile(const QString &filename)
{
    QMimeDatabase mdb;
    return mdb.mimeTypeForFile(filename);
}

void MimeDatabase::addMimeType(const QString &mimetype)
{
    QMimeDatabase mdb;
    MimeType mt = mdb.mimeTypeForName(mimetype);
    if (mt.isValid() && !s_mimetypes.contains(mt)) {
        s_mimetypes.push_back(mt);
    }
}

QString MimeDatabase::allFiltersString()
{
    QSet<QString> uniqueFilters;
    foreach (const MimeType &mimetype, s_mimetypes) {
        const QString &filterString = mimetype.filterString();
        if (!filterString.isEmpty())
            uniqueFilters.insert(filterString);
    }

    QStringList filters;
    foreach (const QString &filter, uniqueFilters)
        filters.append(filter);
    filters.sort();

    return filters.join(";;");
}

} // namespace Utils
