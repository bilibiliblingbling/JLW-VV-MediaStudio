#ifndef MIME_DATABASE_H
#define MIME_DATABASE_H

#include <QMimeType>

#include "utils_global.h"

namespace Utils {

typedef QMimeType MimeType;

class MEDIASTUDIO_UTILS_EXPORT MimeDatabase
{
public:
    // for mediastudio
    static MimeType mimeTypeForName(const QString &mimetype);
    static MimeType mimeTypeForFile(const QString &filename);
    static void addMimeType(const QString &mimetype);
    static QString allFiltersString();
};


} // namespace Utils

#endif // MIME_DATABASE_H
