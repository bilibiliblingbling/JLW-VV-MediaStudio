#ifndef META_MIME_DATA_H
#define META_MIME_DATA_H

#include <QMimeData>

#include <core/id.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {

class MetaData;
namespace Internal { class MetaMimeDataPrivate; }
class PROJECTEXPLORER_EXPORT MetaMimeData : public QMimeData
{
    Q_OBJECT

public:
    MetaMimeData();
    virtual ~MetaMimeData();

public:
    bool hasFormat(const QString &mimetype) const Q_DECL_OVERRIDE;

public:
    void addMetaData(MetaData *metadata);
    QList<MetaData *> metaDatas(const QString &mimetype = QLatin1String("")) const;

private:
    Internal::MetaMimeDataPrivate *d;
};

} // namespace ProjectExplorer

#endif // META_MIME_DATA_H
