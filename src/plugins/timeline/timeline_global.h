#ifndef TIME_LINE_GLOBAL_H
#define TIME_LINE_GLOBAL_H

#include <QtGlobal>

#if defined(TIMELINE_LIBRARY)
#  define TIMELINE_EXPORT Q_DECL_EXPORT
#else
#  define TIMELINE_EXPORT Q_DECL_IMPORT
#endif

#endif // TIME_LINE_GLOBAL_H
