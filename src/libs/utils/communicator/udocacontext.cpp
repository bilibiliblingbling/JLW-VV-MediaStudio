#include "udocacontext.h"
#include "utils/tmplog.h"

namespace Utils {

DECL_CLASS_SET(UDOCAContext)
UDOCAContext::UDOCAContext(QObject *parent)
    : QObject(parent)
{
    HANDLE_CONSTRUCT(UDOCAContext);
}

UDOCAContext::~UDOCAContext()
{
    HANDLE_DESTRUCT(UDOCAContext);
}

// WARNING: GNU only
__attribute__((constructor))
void registerMetaTypes()
{
    qRegisterMetaType<UDOCA::Request>("UDOCA::Request");
    qRegisterMetaType<UDOCA::Response>("UDOCA::Response");
}

} // namespace Utils
