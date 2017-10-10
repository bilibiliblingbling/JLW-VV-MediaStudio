#ifndef MEDIASOURCE_CAMERA_TREE_H
#define MEDIASOURCE_CAMERA_TREE_H

#include "devicetree.h"
#include "mediasource_global.h"

namespace MediaSource {

class MEDIASOURCE_EXPORT CameraTree : public DeviceTree
{
    Q_OBJECT

public:
    explicit CameraTree(QObject *parent = 0);
    virtual ~CameraTree();
};

} // namespace MediaSource

#endif // MEDIASOURCE_CAMERA_TREE_H
