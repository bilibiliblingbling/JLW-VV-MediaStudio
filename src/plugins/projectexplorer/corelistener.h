#ifndef PROJECT_EXPLORER_CORE_LISTENER_H
#define PROJECT_EXPLORER_CORE_LISTENER_H

#include <core/icorelistener.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {
namespace Internal {

class CoreListener : public Core::ICoreListener
{
    Q_OBJECT

public:
    CoreListener();

public:
    bool coreAboutToClose();
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // PROJECT_EXPLORER_CORE_LISTENER_H
