#ifndef CAMERA_PROPERTY_DIALOG_H
#define CAMERA_PROPERTY_DIALOG_H

#include <QDialog>

namespace KingVision {
namespace Internal {

class Camera;

class CameraPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    CameraPropertyDialog(const Camera &camera, QWidget *parent = 0);

};

} // namespace Internal
} // namespace KingVision
#endif // CAMERA_PROPERTY_DIALOG_H
