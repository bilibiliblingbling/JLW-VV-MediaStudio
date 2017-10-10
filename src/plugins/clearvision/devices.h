#ifndef CLEARVISION_DEVICES_H
#define CLEARVISION_DEVICES_H

#include <UDOCA/Language.h>

#include <core/id.h>
#include <projectexplorer/camera.h>

#include "clearvisionnodes.h"

namespace ClearVision {
namespace Internal {

namespace PE = ProjectExplorer;

class Domain;
class UObject
{
public:
    virtual ~UObject();

public:
    static UObject *create(const UDOCA::Object &obj);

public:
    virtual bool isValid() const = 0;

    virtual UDOCA::String objId() const = 0;
    virtual Core::Id objIId() const = 0;

    virtual QString objName() const = 0;
    virtual quint32 objClass() const = 0;
    virtual UDOCA::String objPath() const = 0;

    virtual UDOCA::String objUnitId() const = 0;
    virtual Core::Id objUnitIId() const = 0;

public:
    virtual bool isCamera() const = 0;
    virtual qint32 priority() const = 0;

public:
    virtual PE::Camera *asCamera();
    virtual Domain *asDomain();

public:
    virtual ObjectNode *createNode() const = 0;
};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_DEVICES_H
