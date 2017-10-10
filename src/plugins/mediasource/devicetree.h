#ifndef MEDIASOURCE_DEVICE_TREE_H
#define MEDIASOURCE_DEVICE_TREE_H

#include <core/id.h>
#include <projectexplorer/basictree.h>

#include "mediasource_global.h"

namespace ProjectExplorer { class MediaSource; }

namespace MediaSource {

class MEDIASOURCE_EXPORT DeviceTree : public ProjectExplorer::BasicTree
{
    Q_OBJECT

public:
    explicit DeviceTree(QObject *parent = 0);
    virtual ~DeviceTree();

public:
    virtual void setCurrentMediaSource(ProjectExplorer::MediaSource *mediasource);

signals:
    void aboutToQueryCameras(const QList<Core::Id> &cameras);
};

} // namespace MediaSource

#endif // MEDIASOURCE_DEVICE_TREE_H
