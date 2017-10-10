#include <player/iplayer.h>
#include <player/iplayerfactory.h>
#include <projectexplorer/metamimedata.h>
#include <projectexplorer/metadata.h>
#include <extensionsystem/pluginmanager.h>

#include "playerwidget.h"

namespace Monitor {
namespace Internal {

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
{
    setBackground(Qt::lightGray);
    setAcceptDrops(true);
}

PlayerWidget::~PlayerWidget()
{

}

HWND PlayerWidget::getWinId() const
{
    return (HWND)this->winId();
}

void PlayerWidget::dragEnterEvent(QDragEnterEvent *event)
{
   event->acceptProposedAction();
}

void PlayerWidget::dropEvent(QDropEvent *event)
{
    const ProjectExplorer::MetaMimeData *mimeData =
            (ProjectExplorer::MetaMimeData*)(event->mimeData());
    const QList<ProjectExplorer::MetaData *> dropMesList = mimeData->metaDatas();

    auto playerfactories
            = ExtensionSystem::PluginManager::getObjects<Player::IPlayerFactory>();

    foreach (auto metadata, dropMesList) {
        foreach (Player::IPlayerFactory *factory, playerfactories) {
            if (factory->mimeTypes().contains(metadata->mimeType())) {
                m_player = factory->createPlayer();
                if(!m_player)
                    return;

                m_player->setScreen(this);
                this->setAttribute(Qt::WA_PaintOnScreen);
                m_player->aboutToPlayMetaData(metadata);
            }
        }
    }
}

void PlayerWidget::setBackground(QColor color)
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

} // namespace Internal
} // namespace Monitor
