#ifndef GENERIC_PROJECT_MANAGER_GLOBAL_H
#define GENERIC_PROJECT_MANAGER_GLOBAL_H

#include <QtGlobal>

#if defined(GENERICPROJECTMANAGER_LIBRARY)
#  define GENERICPROJECTMANAGER_EXPORT Q_DECL_EXPORT
#else
#  define GENERICPROJECTMANAGER_EXPORT Q_DECL_IMPORT
#endif

#endif // GENERIC_PROJECT_MANAGER_GLOBAL_H
