#include <projectexplorer/metamimedata.h>
#include <projectexplorer/metadata.h>
#include <projectexplorer/segment.h>
#include <QDebug>
#include <QHBoxLayout>

#include "tempwidget.h"
#include "segmentplayer.h"


tempWidget::tempWidget(QWidget *parent) : QWidget(parent)
{
    segPlay = new ClearVision::Internal::SegmentPlayer();
    segPlay->setScreen(this);
    setAcceptDrops(true);
    resize(500,400);
}

void tempWidget::dragEnterEvent(QDragEnterEvent *event)
{
     event->acceptProposedAction();
}

void tempWidget::dropEvent(QDropEvent *event)
{
    //qDebug() << "PreviewPanel::dropEvent: " << m_currentWidget;
    const ProjectExplorer::MetaMimeData *mimeData =
            (ProjectExplorer::MetaMimeData*)(event->mimeData());
    const QList<ProjectExplorer::MetaData *> dropMesList = mimeData->metaDatas();

    foreach (auto metadata, dropMesList) {
        segPlay->aboutToPlayMetaData(metadata);
        m_bgnTime = qobject_cast<ProjectExplorer::Segment *>(metadata)->bgnTime();
        m_endTime = qobject_cast<ProjectExplorer::Segment *>(metadata)->endTime();
        qDebug()<<"m_bgnTime:"<<m_bgnTime<<m_endTime;
    }
}

HWND tempWidget::getWinId() const
{
    return (HWND)(this->winId());
}

void tempWidget::showVideo()
{

}

void tempWidget::showBackGround()
{

}

void tempWidget::showMessage(const QString &msg)
{

}

