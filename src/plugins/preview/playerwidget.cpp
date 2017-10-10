#include <QDrag>
#include <clearvision/segmentplayer.h>
#include <projectexplorer/metamimedata.h>
#include <projectexplorer/metadata.h>
#include <extensionsystem/pluginmanager.h>
#include <player/iplayer.h>
#include <player/iplayerfactory.h>
#include "playerwidget.h"

namespace Preview {
namespace Internal {

PlayerWidget::PlayerWidget(QWidget *parent)
    : BasicWidget(parent)
{
    //setAttribute(Qt::WA_PaintOnScreen);
    setWidgetBackground(Qt::lightGray);
    setAcceptDrops(true);

#if 0 //drag
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << 500 << 400;//setInTime and setOutTime

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
#endif
}

PlayerWidget::~PlayerWidget()
{

}

HWND PlayerWidget::getWinId() const
{
    return (HWND)this->winId();
}

void PlayerWidget::showVideo()
{

}

void PlayerWidget::showBackGround()
{

}

void PlayerWidget::showMessage(const QString &msg)
{

}

void PlayerWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void PlayerWidget::dropEvent(QDropEvent *event)
{
    emit currentWidgetAcceptDropEvent();

    const ProjectExplorer::MetaMimeData *mimeData =
           (ProjectExplorer::MetaMimeData*)(event->mimeData());
    const QList<ProjectExplorer::MetaData *> dropMesList = mimeData->metaDatas();

    auto playerfactories
            = ExtensionSystem::PluginManager::getObjects<Player::IPlayerFactory>();

    foreach (auto metadata, dropMesList) {
        qDebug() << "dropEvent: " << metadata->id() << metadata->toMap() << metadata->mimeType();

        foreach (Player::IPlayerFactory *factory, playerfactories) {
            qDebug() << "factory->mineTypes: " << factory->mimeTypes();
            if (factory->mimeTypes().contains(metadata->mimeType())) {
                Player::IPlayerPtr player = factory->createPlayer();
                if(!player)
                    return;
                //qDebug() << "dropevent player: " << &m_player << "bgnTime: " << m_player->begin();
                player->setScreen(this);
                this->setAttribute(Qt::WA_PaintOnScreen);
                player->aboutToPlayMetaData(metadata);

                emit createPlayerSuccessed(player);
            }
        }
    }
    qDebug() << "dropEvent end";
}

} // namespace Internal
} // namespace Preview

