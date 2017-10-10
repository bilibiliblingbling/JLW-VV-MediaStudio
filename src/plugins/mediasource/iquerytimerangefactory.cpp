#include <QCoreApplication>

#include "constants.h"
#include "iquerytimerangefactory.h"

namespace MediaSource {
namespace {
static QTime s_midnight(0, 0, 0, 0);
QDateTime dateTimeFromToday(int dspan = 0, int mspan = 0, int yspan = 0, bool clearTime = false)
{
    QDateTime dt = QDateTime::currentDateTime();
    if (yspan)
        dt = dt.addYears(-yspan);
    if (mspan)
        dt = dt.addMonths(-mspan);
    if (dspan)
        dt = dt.addDays(-dspan);
    if (clearTime)
        dt.setTime(s_midnight);
    return dt;
}
} // namespace anonymous

class QueryTimeRangeCustomized : public IQueryTimeRangeFactory
{
public:
    QueryTimeRangeCustomized(int priority = 0)
    {
        setDisplayName(QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_CUSTOMIZED));
        setId(Constants::MEDIASOURCE_QUERY_CUSTOMIZED);
        setPriority(priority);
    }

public:
    bool dynamicTimeRange() const Q_DECL_OVERRIDE { return false; }
    void updateTimeRange(const QDateTime &timeBgn, const QDateTime &timeEnd) Q_DECL_OVERRIDE
    {
        if (timeBgn.isValid())
            m_timeBgn = timeBgn;
        if (timeEnd.isValid())
            m_timeEnd = timeEnd;
    }

public:
    QDateTime queryTimeBegin() const Q_DECL_OVERRIDE
    {
        if (!m_timeBgn.isValid())
            m_timeBgn = dateTimeFromToday(1);
        return m_timeBgn;
    }

    QDateTime queryTimeEnd() const Q_DECL_OVERRIDE
    {
        if (!m_timeEnd.isValid())
            m_timeEnd = dateTimeFromToday();
        return m_timeEnd;
    }

private:
    mutable QDateTime m_timeBgn;
    mutable QDateTime m_timeEnd;
};

template<typename FunctionBgnTime, typename FunctionEndTime>
class QueryTimeRange : public IQueryTimeRangeFactory
{
public:
    QueryTimeRange(FunctionBgnTime funcBgn, FunctionEndTime funcEnd, const QString &name, Core::Id id, int priority = 0)
        : m_bgnTimeFunc(funcBgn), m_endTimeFunc(funcEnd)
    {
        setDisplayName(name);
        setId(id);
        setPriority(priority);
    }

public:
    QDateTime queryTimeBegin() const
    {
        return m_bgnTimeFunc();
    }

    QDateTime queryTimeEnd() const
    {
        return m_endTimeFunc();
    }

private:
    FunctionBgnTime m_bgnTimeFunc;
    FunctionEndTime m_endTimeFunc;
};

////////////////////////////////////////////////////////////////////////////////

namespace Internal {
class QueryTimeRangeFactoryManagerPrivate
{
public:
    QHash<Core::Id, IQueryTimeRangeFactory *> m_factories;
};
} // namespace Internal

static QueryTimeRangeFactoryManager *s_querytimerangefactorymanager = 0;

QueryTimeRangeFactoryManager::QueryTimeRangeFactoryManager(QObject *parent)
    : QObject(parent)
    , d(new Internal::QueryTimeRangeFactoryManagerPrivate())
{
    setObjectName(QLatin1String("MediaSource.QueryTimeRangeFactoryManager"));

    {
        // customized
        IQueryTimeRangeFactory *factory = 0;
        factory = new QueryTimeRangeCustomized(0);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // today
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(0, 0, 0, true);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_TODAY),
                    Core::Id(Constants::MEDIASOURCE_QUERY_TODAY),
                    1);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // this week
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(QDate::currentDate().dayOfWeek()-1, 0, 0, true);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_THISWEEK),
                    Core::Id(Constants::MEDIASOURCE_QUERY_THISWEEK),
                    2);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // this month
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(QDate::currentDate().day()-1, 0, 0, true);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_THISMONTH),
                    Core::Id(Constants::MEDIASOURCE_QUERY_THISMONTH),
                    3);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 1 day
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(1);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_1DAY),
                    Core::Id(Constants::MEDIASOURCE_QUERY_1DAY),
                    4);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 3 days
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(3);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_3DAYS),
                    Core::Id(Constants::MEDIASOURCE_QUERY_3DAYS),
                    5);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 7 days
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(7);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_7DAYS),
                    Core::Id(Constants::MEDIASOURCE_QUERY_7DAYS),
                    6);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 15 days
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(15);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_15DAYS),
                    Core::Id(Constants::MEDIASOURCE_QUERY_15DAYS),
                    7);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 30 days
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(30);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_30DAYS),
                    Core::Id(Constants::MEDIASOURCE_QUERY_30DAYS),
                    8);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 3 months
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(0, 3);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_3MONTHS),
                    Core::Id(Constants::MEDIASOURCE_QUERY_3MONTHS),
                    9);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 6 months
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(0, 6);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_6MONTHS),
                    Core::Id(Constants::MEDIASOURCE_QUERY_6MONTHS),
                    10);
        d->m_factories.insert(factory->id(), factory);
    }

    {
        // 1 year
        IQueryTimeRangeFactory *factory = 0;
        auto funcBgn = []() -> QDateTime {
                return dateTimeFromToday(0, 0, 1);
        };
        auto funcEnd = []() -> QDateTime {
                return dateTimeFromToday();
        };

        factory = new QueryTimeRange<decltype(funcBgn), decltype(funcEnd)>(
                    funcBgn, funcEnd,
                    QCoreApplication::translate("MediaSource", Constants::TR_TIMERANGE_1YEAR),
                    Core::Id(Constants::MEDIASOURCE_QUERY_1YEAR),
                    11);
        d->m_factories.insert(factory->id(), factory);
    }

    s_querytimerangefactorymanager = this;
}

QueryTimeRangeFactoryManager::~QueryTimeRangeFactoryManager()
{
    s_querytimerangefactorymanager = 0;

    qDeleteAll(d->m_factories);
    delete d;
}

QueryTimeRangeFactoryManager *QueryTimeRangeFactoryManager::instance()
{
    return s_querytimerangefactorymanager;
}

QList<IQueryTimeRangeFactory *> QueryTimeRangeFactoryManager::factories()
{
    return s_querytimerangefactorymanager->d->m_factories.values();
}

IQueryTimeRangeFactory *QueryTimeRangeFactoryManager::factory(Core::Id id)
{
    return s_querytimerangefactorymanager->d->m_factories.value(id);
}

////////////////////////////////////////////////////////////////////////////////

IQueryTimeRangeFactory::~IQueryTimeRangeFactory()
{}

QString IQueryTimeRangeFactory::displayName() const
{
    return m_displayName;
}

int IQueryTimeRangeFactory::priority() const
{
    return m_priority;
}

Core::Id IQueryTimeRangeFactory::id() const
{
    return m_id;
}

bool IQueryTimeRangeFactory::dynamicTimeRange() const
{
    return true;
}

void IQueryTimeRangeFactory::updateTimeRange(const QDateTime &, const QDateTime &)
{
}

void IQueryTimeRangeFactory::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

void IQueryTimeRangeFactory::setPriority(int priority)
{
    m_priority = priority;
}

void IQueryTimeRangeFactory::setId(Core::Id id)
{
    m_id = id;
}

} // namespace MediaSource
