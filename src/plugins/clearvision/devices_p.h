#ifndef CLEARVISION_DEVICES_P_H
#define CLEARVISION_DEVICES_P_H

#include <core/id.h>
#include <projectexplorer/camera.h>
#include <utils/communicator/udocaobjecthelper.h>

#include "clearvisionnodes_p.h"
#include "constants.h"
#include "devices.h"

namespace ClearVision {
namespace Internal {

template<typename T>
class UObjectImpl : public T, public UObject
{
public:
    typedef UObjectImpl<T> BaseType;

public:
    UObjectImpl(const UDOCA::Object &obj)
        : m_object(obj)
    {
        T::setId(objIId());
    }

public:
    bool isValid() const Q_DECL_OVERRIDE
    {
        return T::isValid();
    }

    UDOCA::String objId() const Q_DECL_OVERRIDE
    {
        return Utils::safeUStringProperty(m_object, Constants::CLEARVISION_OBJECT_ID);
    }

    Core::Id objIId() const Q_DECL_OVERRIDE
    {
        return Core::Id::fromString(objId().getData());
    }

    QString objName() const Q_DECL_OVERRIDE
    {
        return Utils::safeStringProperty(m_object, Constants::CLEARVISION_OBJECT_NAME);
    }

    quint32 objClass() const Q_DECL_OVERRIDE
    {
        return Utils::safeUInt32Property(m_object, Constants::CLEARVISION_OBJECT_CLASS);
    }

    UDOCA::String objPath() const Q_DECL_OVERRIDE
    {
        return Utils::safeUStringProperty(m_object, Constants::CLEARVISION_OBJECT_ORGPATH);
    }

    UDOCA::String objUnitId() const Q_DECL_OVERRIDE
    {
        return Utils::safeUStringProperty(m_object, Constants::CLEARVISION_OBJECT_ORGUNITID);
    }

    Core::Id objUnitIId() const Q_DECL_OVERRIDE
    {
        return Core::Id::fromString(objUnitId().getData());
    }

public:
    bool isCamera() const Q_DECL_OVERRIDE { return false; }
    qint32 priority() const Q_DECL_OVERRIDE { return -1; }

private:
    UDOCA::Object m_object;
};

////////////////////////////////////////////////////////////////////////////////

class Camera : public UObjectImpl<ProjectExplorer::Camera>
{
    Q_OBJECT

public:
    Camera(const UDOCA::Object &obj);

public:
    QString name() const Q_DECL_OVERRIDE;

public:
    bool isCamera() const Q_DECL_OVERRIDE;
    qint32 priority() const Q_DECL_OVERRIDE;

public:
    PE::Camera *asCamera() Q_DECL_OVERRIDE;

public:
    ObjectNode *createNode() const Q_DECL_OVERRIDE;
};

class Unit : public UObjectImpl<ProjectExplorer::MetaData>
{
    Q_OBJECT

public:
    Unit(const UDOCA::Object &obj);

public:
    virtual UDOCA::String unitId() const;

public:
    qint32 priority() const Q_DECL_OVERRIDE;

public:
    ObjectNode *createNode() const Q_DECL_OVERRIDE;
};

class Domain : public Unit
{
    Q_OBJECT

public:
    Domain(const UDOCA::Object &obj);

public:
    UDOCA::String unitId() const;

public:
    qint32 priority() const Q_DECL_OVERRIDE;

public:
    Domain *asDomain() Q_DECL_OVERRIDE;

public:
    ObjectNode *createNode() const Q_DECL_OVERRIDE;
};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_DEVICES_P_H
