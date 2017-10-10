#ifndef MEDIA_SOURCE_GLOBAL_H
#define MEDIA_SOURCE_GLOBAL_H

#include <QtGlobal>

#if defined(MEDIASOURCE_LIBRARY)
#  define MEDIASOURCE_EXPORT Q_DECL_EXPORT
#else
#  define MEDIASOURCE_EXPORT Q_DECL_IMPORT
#endif

#endif // MEDIA_SOURCE_GLOBAL_H
