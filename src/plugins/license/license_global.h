#ifndef LICENSE_GLOBAL_H
#define LICENSE_GLOBAL_H

#include <QtGlobal>

#if defined(LICENSE_LIBRARY)
#  define LICENSE_EXPORT Q_DECL_EXPORT
#else
#  define LICENSE_EXPORT Q_DECL_IMPORT
#endif

#endif // LICENSE_GLOBAL_H
