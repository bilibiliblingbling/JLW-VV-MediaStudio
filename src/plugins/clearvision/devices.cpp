#include <share/Constants.h>

#include "devices.h"
#include "devices_p.h"

namespace ClearVision {
namespace Internal {
namespace {

quint32 objectClass(const UDOCA::Object &obj)
{
    return Utils::safeUInt32Property(obj, Constants::CLEARVISION_OBJECT_CLASS);
}

} // namespace anonymous

UObject::~UObject()
{}

UObject *UObject::create(const UDOCA::Object &obj)
{
    UObject *o = 0;
    switch (objectClass(obj)) {
    case CV_OBJECT_CLASS_CAMERA:
    case CV_OBJECT_CLASS_NETWORK_CAMERA:
        o = new Camera(obj);
        break;
    case CV_OBJECT_CLASS_OU:
        o = new Unit(obj);
        break;
    case CV_OBJECT_CLASS_DOMAIN:
        o = new Domain(obj);
        break;
    default:
        break;
    }

    if (o && !o->isValid()) {
        delete o;
        o = 0;
    }

    return o;
}

PE::Camera *UObject::asCamera()
{
    return 0;
}

Domain *UObject::asDomain()
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

Camera::Camera(const UDOCA::Object &obj)
    : BaseType(obj)
{
}

QString Camera::name() const
{
    return BaseType::objName();
}

bool Camera::isCamera() const
{
    return true;
}

qint32 Camera::priority() const
{
    return Constants::CLEARVISION_CAMERA_PRIORITY;
}

ProjectExplorer::Camera *Camera::asCamera()
{
    return this;
}

ObjectNode *Camera::createNode() const
{
    return new ObjectNodeImpl<Camera>(this);
}

////////////////////////////////////////////////////////////////////////////////

Unit::Unit(const UDOCA::Object &obj)
    : BaseType(obj)
{
}

UDOCA::String Unit::unitId() const
{
    return BaseType::objId();
}

qint32 Unit::priority() const
{
    return Constants::CLEARVISION_UNIT_PRIORITY;
}

ObjectNode *Unit::createNode() const
{
    return new ObjectNodeImpl<Unit>(this);
}

////////////////////////////////////////////////////////////////////////////////

Domain::Domain(const UDOCA::Object &obj)
    : Unit(obj)
{
}

UDOCA::String Domain::unitId() const
{
    return "/";
}

qint32 Domain::priority() const
{
    return Constants::CLEARVISION_DOMAIN_PRIORITY;
}

Domain *Domain::asDomain()
{
    return this;
}

ObjectNode *Domain::createNode() const
{
    return new ObjectNodeImpl<Domain>(this);
}

} // namespace Internal
} // namespace ClearVision
