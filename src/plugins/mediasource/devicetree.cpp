#include "devicetree.h"

namespace MediaSource {

DeviceTree::DeviceTree(QObject *parent)
    : ProjectExplorer::BasicTree(parent)
{
}

DeviceTree::~DeviceTree()
{
}

void DeviceTree::setCurrentMediaSource(ProjectExplorer::MediaSource *)
{
}

} // namespace MediaSource
