#include "constants.h"
#include "playerfactory.h"
#include "segmentplayer.h"

namespace ClearVision {
namespace Internal {

PlayerFactory::PlayerFactory(QObject *parent)
    : Player::IPlayerFactory(parent)
{
    setObjectName(QStringLiteral("ClearVision.PlayerFactory"));
}

PlayerFactory::~PlayerFactory()
{
}

QStringList PlayerFactory::mimeTypes() const
{
    return (QStringList() << QLatin1String(Constants::CLEARVISION_SEGMENT_MIMETYPE));
}

Player::IPlayerPtr PlayerFactory::createPlayer() const
{
    Player::IPlayerPtr player(new SegmentPlayer());
    return player;
}

} // namespace Internal
} // namespace ClearVision
