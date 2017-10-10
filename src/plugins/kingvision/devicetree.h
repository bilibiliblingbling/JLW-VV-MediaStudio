#ifndef KINGVISION_DEVICE_TREE_H
#define KINGVISION_DEVICE_TREE_H

#include <mediasource/devicetree.h>
#include <core/icore.h>

namespace KingVision {
namespace Internal {

namespace PE = ProjectExplorer;
namespace MS = MediaSource;

class Codec;
class Camera;
class DeviceTreePrivate;
class DeviceTreeWidget;
class MediaSource;
class DeviceTree : public MS::DeviceTree
{
    Q_OBJECT

public:
    DeviceTree(QObject *parent = 0);
    ~DeviceTree();

    static DeviceTree *instance();

public:
    void setCurrentMediaSource(PE::MediaSource *mediasource);

public:
    ProjectExplorer::FolderNode *rootNode() Q_DECL_OVERRIDE;
    ProjectExplorer::Node *currentNode() Q_DECL_OVERRIDE;

    void nodeChanged(ProjectExplorer::BasicTreeWidget *widget) Q_DECL_OVERRIDE;
    void showContextMenu(ProjectExplorer::BasicTreeWidget *focus, const QPoint &globalPos, ProjectExplorer::Node *node) Q_DECL_OVERRIDE;
    bool dragableInTree(ProjectExplorer::Node *node) const Q_DECL_OVERRIDE;

signals:
    void currentNodeChanged(ProjectExplorer::Node *node, MediaSource *currentMediaSource);

private:
    void aboutToQueryCurrentCameras();

    void aboutToStartRecordingCurrentCamera();
    void aboutToStopRecordingCurrentCamera();

    void aboutToStartRecordingAll();
    void aboutToStopRecordingAll();

    void displayDeviceProperties();
    void displayDevicePraramWizard();

    void removeDevice();
private:
    void update(ProjectExplorer::Node *node, MediaSource *currentMediaSource);

private:
    void codecAboutToBeAdded(Codec *codec);
    void codecAdded();

    void cameraAboutToBeAdded(Camera *camera);
    void cameraAdded();

    void codecAboutToBeRemoved(Codec *codec);
    void codecRemoved();

    void codecsAboutToBeCleared();
    void codecsCleared();

private:
    void codecAddedImpl(Codec *codec);
    void cameraAddedImpl(Camera *camera);

private:
    DeviceTreePrivate *d;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_DEVICE_TREE_H
