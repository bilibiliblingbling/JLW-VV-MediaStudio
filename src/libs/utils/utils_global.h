#ifndef UTILS_GLOBAL_H
#define UTILS_GLOBAL_H

#include <QtGlobal>

#if defined(MEDIASTUDIO_UTILS_LIBRARY)
#  define MEDIASTUDIO_UTILS_EXPORT Q_DECL_EXPORT
#else
#  define MEDIASTUDIO_UTILS_EXPORT Q_DECL_IMPORT
#endif

namespace Utils {

} // namespace Utils

#endif // UTILS_GLOBAL_H
