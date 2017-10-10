#ifndef CLEARVISION_NODES_P_H
#define CLEARVISION_NODES_P_H

#include <resource/resource.h>
#include <share/Constants.h>

#include "clearvisionnodes.h"

namespace ClearVision {
namespace Internal {

template<typename T>
class ObjectNodeImpl : public ObjectNode
{
public:
    ObjectNodeImpl(const T * const obj)
        : m_object(obj)
    {
        if (!isCamera()) {
            StatusNode *node = new StatusNode();
            addNode(node);
        }
    }

public:
    bool isCamera() const Q_DECL_OVERRIDE
    {
        return m_object->isCamera();
    }

    Core::Id unitId() const Q_DECL_OVERRIDE
    {
        return m_object->objUnitIId();
    }

public:
    QString displayName() const Q_DECL_OVERRIDE
    {
        return m_object->objName();
    }

    QIcon icon() const Q_DECL_OVERRIDE
    {
        int idx = 0;

        switch (m_object->objClass()) {
        case CV_OBJECT_CLASS_CAMERA:
        case CV_OBJECT_CLASS_NETWORK_CAMERA:
            idx = Resource::Constants::IDX_CAMERA;
            break;
        case CV_OBJECT_CLASS_OU:
            idx = Resource::Constants::IDX_FOLDER;
            break;
        case CV_OBJECT_CLASS_DOMAIN:
            idx = Resource::Constants::IDX_DOMAIN;
            break;
        default:
            Q_ASSERT(0);
            break;
        }

        return Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, idx);
    }

public:
    T *metadata() const Q_DECL_OVERRIDE
    {
        return const_cast<T *>(m_object);
    }

public:
    qint32 priority() const Q_DECL_OVERRIDE
    {
        return m_object->priority();
    }

private:
    const T * const m_object;
};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_NODES_P_H
