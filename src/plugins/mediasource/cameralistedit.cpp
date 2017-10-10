#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>

#include <core/icore.h>
#include <projectexplorer/camera.h>
#include <utils/tmplog.h>

#include "cameralistedit.h"
#include "cameralistedit_p.h"

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

CameraListEdit::CameraListEdit(QWidget *parent)
    : QLineEdit(parent)
    , d(new Internal::CameraListEditPrivate())
{
    d->m_dialog = new PE::SelectTreeDialog(this);
    d->m_cameraWidget = new TreeWidget(this);
    d->m_dialog->setBasicTreeWidget(d->m_cameraWidget);
    connect(d->m_dialog, SIGNAL(hidingSelectTree(QList<ProjectExplorer::Node*>,bool)),
            this, SLOT(setCamerasImpl(QList<ProjectExplorer::Node*>,bool)));

    setContextMenuPolicy(Qt::NoContextMenu);
    setReadOnly(true);
}

CameraListEdit::~CameraListEdit()
{
    delete d;
}

QList<Core::Id> CameraListEdit::cameras()
{
    return d->m_cameras.keys();
}

void CameraListEdit::setCameras(const QList<Core::Id> &cameras)
{
    d->m_dialog->clearSelection();
    d->m_dialog->selectNodes(cameras);
    setCamerasImpl(d->m_dialog->selectedNodes());
}

void CameraListEdit::setCameraTree(ProjectExplorer::BasicTree *cameratree)
{
    if (d->m_cameraWidget) {
        d->m_cameraWidget->setBasicTree(cameratree);
    }
}

void CameraListEdit::mousePressEvent(QMouseEvent *e)
{
    if (d->m_dialog) {
        e->accept();

        initSelectTreePopup();
        positionSelectTree();
        d->m_dialog->show();
    } else {
        QLineEdit::mousePressEvent(e);
    }
}

void CameraListEdit::setCamerasImpl(const QList<ProjectExplorer::Node *> &cameraNodes, bool append)
{
    if (!append)
        d->m_cameras.clear();

    foreach (auto camera, cameraNodes) {
        if (camera && camera->isValid()
            && qobject_cast<PE::Camera *>(camera->metadata())
            && !d->m_cameras.contains(camera->id())) {
            d->m_cameras.insert(camera->id(), camera);
        }
    }
    updateText();
}

void CameraListEdit::updateText()
{
    QStringList cameras;
    foreach (auto camera, d->m_cameras) {
        cameras << camera->displayName();
    }
    setText(cameras.join(QLatin1String("; ")));
    setToolTip(cameras.join(QLatin1String(";\n")));

    home(false);
}

void CameraListEdit::initSelectTreePopup()
{
    syncSelectTree();
}

void CameraListEdit::positionSelectTree()
{
    QPoint pos = (layoutDirection() == Qt::RightToLeft)? rect().bottomRight(): rect().bottomLeft();
    QPoint pos2 = (layoutDirection() == Qt::RightToLeft)? rect().topRight(): rect().topLeft();
    pos = mapToGlobal(pos);
    pos2 = mapToGlobal(pos2);
    QSize size = d->m_dialog->sizeHint();
    QRect screen = QApplication::desktop()->availableGeometry(pos);

    //handle popup falling "off screen"
    if (layoutDirection() == Qt::RightToLeft) {
        pos.setX(pos.x()-size.width());
        pos2.setX(pos2.x()-size.width());
        if (pos.x() < screen.left())
            pos.setX(qMax(pos.x(), screen.left()));
        else if (pos.x()+size.width() > screen.right())
            pos.setX(qMax(pos.x()-size.width(), screen.right()-size.width()));
    } else {
        if (pos.x()+size.width() > screen.right())
            pos.setX(screen.right()-size.width());
        pos.setX(qMax(pos.x(), screen.left()));
    }

    if (pos.y() + size.height() > screen.bottom())
        pos.setY(pos2.y() - size.height());
    else if (pos.y() < screen.top())
        pos.setY(screen.top());
    if (pos.y() < screen.top())
        pos.setY(screen.top());
    if (pos.y()+size.height() > screen.bottom())
        pos.setY(screen.bottom()-size.height());

    d->m_dialog->move(pos);
}

void CameraListEdit::syncSelectTree()
{
    d->m_dialog->clearSelection();
    d->m_dialog->selectNodes(d->m_cameras.values());
    updateText();
}

} // namespace Internal
} // namespace MediaSource
