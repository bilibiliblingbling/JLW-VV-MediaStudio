#include "udocaobjecthelper.h"
#include "utils/tmplog.h"

namespace Utils {

qint32 safeInt32Property(const UDOCA::Object &obj, const char *property, bool *bOk)
{
    return safeProperty<qint32>(obj, property, bOk);
}

quint32 safeUInt32Property(const UDOCA::Object &obj, const char *property, bool *bOk)
{
    return safeProperty<quint32>(obj, property, bOk);
}

quint64 safeUInt64Property(const UDOCA::Object &obj, const char *property, bool *bOk)
{
    return (quint64)(uint64_t)safeProperty<UDOCA::UInt64>(obj, property, bOk);
}

UDOCA::String safeUStringProperty(const UDOCA::Object &obj, const char *property)
{
    return safeProperty<UDOCA::String>(obj, property);
}

QString safeStringProperty(const UDOCA::Object &obj, const char *property)
{
    return safeUStringProperty(obj, property).getData();
}

QDateTime safeDateTimeProperty(const UDOCA::Object &obj, const char *property)
{
    return QDateTime::fromString(safeProperty<UDOCA::DateTime>(obj, property).toString().getData(),
                                 QStringLiteral("yyyy-MM-dd hh:mm:ss"));
}

QDateTime safeGMTDateTimeProperty(const UDOCA::Object &obj, const char *property)
{
    return QDateTime::fromMSecsSinceEpoch(safeProperty<UDOCA::DateTime>(obj, property).toDateTime().toUnixTime() * 1000);
}

} // namespace Utils
