#ifndef PLAYER_GLOBAL_H
#define PLAYER_GLOBAL_H

#include <QtGlobal>

#if defined(PLAYER_LIBRARY)
#  define PLAYER_EXPORT Q_DECL_EXPORT
#else
#  define PLAYER_EXPORT Q_DECL_IMPORT
#endif

namespace Player {

} // namespace Player

#endif // PLAYER_GLOBAL_H
