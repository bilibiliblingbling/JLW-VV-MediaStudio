#ifndef UTILS_COMM_UDOCA_OBJECT_HELPER_H
#define UTILS_COMM_UDOCA_OBJECT_HELPER_H

#include <QDateTime>

#include <UDOCA/Language.h>

#include "utils/tmplog.h"
#include "utils/utils_global.h"

namespace Utils {

MEDIASTUDIO_UTILS_EXPORT qint32 safeInt32Property(const UDOCA::Object &obj, const char *property, bool *bOk = 0);

MEDIASTUDIO_UTILS_EXPORT quint32 safeUInt32Property(const UDOCA::Object &obj, const char *property, bool *bOk = 0);
MEDIASTUDIO_UTILS_EXPORT quint64 safeUInt64Property(const UDOCA::Object &obj, const char *property, bool *bOk = 0);

MEDIASTUDIO_UTILS_EXPORT UDOCA::String safeUStringProperty(const UDOCA::Object &obj, const char *property);
MEDIASTUDIO_UTILS_EXPORT QString safeStringProperty(const UDOCA::Object &obj, const char *property);

MEDIASTUDIO_UTILS_EXPORT QDateTime safeDateTimeProperty(const UDOCA::Object &obj, const char *property);
MEDIASTUDIO_UTILS_EXPORT QDateTime safeGMTDateTimeProperty(const UDOCA::Object &obj, const char *property);


template <typename T>
MEDIASTUDIO_UTILS_EXPORT T safeProperty(const UDOCA::Object &obj, const char *property, const T &defVal)
{
    T val = defVal;

    try {
        if (obj.isValid() && obj.hasProperty(property))
            val = (T)obj.getProperty(property);
    } catch (const UDOCA::Exception &e) {
        TM_DBG("failed to get value of `%s': %s", property, e.getMessage().getData());
    } catch (...) {
        TM_DBG("failed to get value of `%s', with unhandled exception", property);
    }

    return val;
}

template<typename T>
MEDIASTUDIO_UTILS_EXPORT T safeProperty(const UDOCA::Object &obj, const char *property, bool *bOk = 0)
{
    T val;

    if (bOk)
        *bOk = false;

    try {
        if (obj.isValid() && obj.hasProperty(property)) {
            val = (T)obj.getProperty(property);
            if (bOk)
                *bOk = true;
        }
    } catch (const UDOCA::Exception &e) {
        TM_DBG("failed to get value of `%s': %s", property, e.getMessage().getData());
    } catch (...) {
        TM_DBG("failed to get value of `%s', with unhandled exception", property);
    }

    return val;
}

} // namespace Utils

#endif // UTILS_COMM_UDOCA_OBJECT_HELPER_H
