#ifndef CAMERA_LIST_EDIT_H
#define CAMERA_LIST_EDIT_H

#include <QLineEdit>

#include <core/id.h>

namespace ProjectExplorer {
class Node;
class BasicTree;
}

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class CameraListEditPrivate;
class CameraListEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit CameraListEdit(QWidget *parent = 0);
    ~CameraListEdit();

public:
    QList<Core::Id> cameras();
    void setCameras(const QList<Core::Id> &cameras);

public:
    void setCameraTree(PE::BasicTree *cameratree);

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    void updateText();

private:
    void initSelectTreePopup();
    void positionSelectTree();
    void syncSelectTree();

private slots:
    void setCamerasImpl(const QList<ProjectExplorer::Node *> &cameraNodes, bool append = false);

private:
    CameraListEditPrivate *d;
};

} // namespace Internal
} // namespace MediaSource

#endif // CAMERA_LIST_EDIT_H
