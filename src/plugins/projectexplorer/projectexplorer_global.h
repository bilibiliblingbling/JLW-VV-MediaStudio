#ifndef PROJECT_EXPLORER_GLOBAL_H
#define PROJECT_EXPLORER_GLOBAL_H

#include <QtGlobal>

#if defined(PROJECTEXPLORER_LIBRARY)
#  define PROJECTEXPLORER_EXPORT Q_DECL_EXPORT
#else
#  define PROJECTEXPLORER_EXPORT Q_DECL_IMPORT
#endif

namespace ProjectExplorer {

} // namespace ProjectExplorer

#endif // PROJECT_EXPLORER_GLOBAL_H
