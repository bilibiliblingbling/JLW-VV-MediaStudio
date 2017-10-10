#ifndef CAMERA_LIST_EDIT_P_H
#define CAMERA_LIST_EDIT_P_H

#include <QPointer>

#include <projectexplorer/basicnodes.h>
#include <projectexplorer/selecttreedialog.h>

#include "treewidget.h"

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class CameraListEditPrivate
{
public:
    PE::SelectTreeDialog *m_dialog;

    QPointer<TreeWidget> m_cameraWidget;

public:
    QHash<Core::Id, ProjectExplorer::Node *> m_cameras;
};

} // namespace Internal
} // namespace MediaSource

#endif // CAMERA_LIST_EDIT_P_H
