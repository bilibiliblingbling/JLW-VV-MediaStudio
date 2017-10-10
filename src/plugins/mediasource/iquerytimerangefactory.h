#ifndef IQUERY_TIME_RANGE_FACTORY_H
#define IQUERY_TIME_RANGE_FACTORY_H

#include <QDateTime>
#include <QObject>

#include <core/id.h>

#include "mediasource_global.h"

namespace MediaSource {

class MEDIASOURCE_EXPORT IQueryTimeRangeFactory
{
public:
    virtual ~IQueryTimeRangeFactory();

public:
    QString displayName() const;
    int priority() const;
    Core::Id id() const;

public:
    virtual bool dynamicTimeRange() const;
    virtual void updateTimeRange(const QDateTime &timeBgn, const QDateTime &timeEnd);

public:
    virtual QDateTime queryTimeBegin() const = 0;
    virtual QDateTime queryTimeEnd() const = 0;

protected:
    void setDisplayName(const QString &displayName);
    void setPriority(int priority);
    void setId(Core::Id id);

private:
    int m_priority;
    Core::Id m_id;
    QString m_displayName;
};


namespace Internal { class QueryTimeRangeFactoryManagerPrivate; }
class MEDIASOURCE_EXPORT QueryTimeRangeFactoryManager : public QObject
{
    Q_OBJECT

public:
    QueryTimeRangeFactoryManager(QObject *parent = 0);
    ~QueryTimeRangeFactoryManager();

    static QueryTimeRangeFactoryManager *instance();

public:
    static QList<IQueryTimeRangeFactory *> factories();
    static IQueryTimeRangeFactory *factory(Core::Id id);

private:
    Internal::QueryTimeRangeFactoryManagerPrivate *d;
};

} // namespace MediaSource

#endif // IQUERY_TIME_RANGE_FACTORY_H
