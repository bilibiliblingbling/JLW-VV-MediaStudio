#ifndef META_DATA_H
#define META_DATA_H

#include <QObject>
#include <QVariantMap>

#include <core/id.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {

class PROJECTEXPLORER_EXPORT MetaData : public QObject
{
    Q_OBJECT

public:
    virtual ~MetaData();

    Core::Id id() const;
    bool isValid() const;

signals:
    void dataChanged();

public:
    virtual QString mimeType() const;

public:
    virtual bool fromMap(const QVariantMap &map);
    virtual QVariantMap toMap() const;

protected:
    MetaData(QObject *parent = 0);
    MetaData(QObject *parent, Core::Id id);
    MetaData(const MetaData &that);

protected:
    void setId(Core::Id id);

private:
    Core::Id m_id;
};

} // namespace ProjectExplorer

#endif // META_DATA_H
