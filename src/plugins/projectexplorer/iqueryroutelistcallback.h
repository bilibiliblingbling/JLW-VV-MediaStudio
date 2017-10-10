#ifndef IQUERY_ROUTELIST_CALLBACK_H
#define IQUERY_ROUTELIST_CALLBACK_H

#include "projectexplorer_global.h"


namespace StreamSDK { class RouteList; }

namespace ProjectExplorer {

class Segment;
class PROJECTEXPLORER_EXPORT IQueryRouteListCallback
{
public:
    virtual ~IQueryRouteListCallback() {}

public:
    virtual void onQueryRouteListSuccess(Segment *segment, const StreamSDK::RouteList &rl) = 0;
    virtual void onQueryRouteListFailed(Segment *segment) = 0;
};



} // namespace ProjectExplorer

#endif // IQUERY_ROUTELIST_CALLBACK_H
