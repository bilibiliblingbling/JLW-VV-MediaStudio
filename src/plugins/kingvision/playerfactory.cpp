#include "cameraplayer.h"
#include "constants.h"
#include "playerfactory.h"
#include "segmentplayer.h"


namespace KingVision {
namespace Internal {

PlayerFactory::PlayerFactory(QObject *parent)
    : Player::IPlayerFactory(parent)
{
    setObjectName(QStringLiteral("KingVision.PlayerFactory"));
}

PlayerFactory::~PlayerFactory()
{
}

QStringList PlayerFactory::mimeTypes() const
{
    return (QStringList()
            << QLatin1String(Constants::KINGVISION_CAMERA_MIMETYPE)
            << QLatin1String(Constants::KINGVISION_SEGMENT_MIMETYPE));
}

Player::IPlayerPtr PlayerFactory::createPlayer() const
{
    Player::IPlayerPtr player;
#if 0
    (new SegmentPlayer());

    if (!player->isValid())
        player.reset();
#endif
    return player;
}


CameraPlayerFactory::CameraPlayerFactory(QObject *parent)
    : Player::IPlayerFactory(parent)
{
    setObjectName(QStringLiteral("KingVision.CameraPlayerFactory"));
}

CameraPlayerFactory::~CameraPlayerFactory()
{
}

QStringList CameraPlayerFactory::mimeTypes() const
{
    return (QStringList() << QLatin1String(Constants::KINGVISION_CAMERA_MIMETYPE));
}

Player::IPlayerPtr CameraPlayerFactory::createPlayer() const
{
    Player::IPlayerPtr player(new CameraPlayer());

    if (player && !player->isValid())
        player.reset();

    return player;
}


SegmentPlayerFactory::SegmentPlayerFactory(QObject *parent)
    : Player::IPlayerFactory(parent)
{
    setObjectName(QStringLiteral("KingVision.SegmentPlayerFactory"));
}

SegmentPlayerFactory::~SegmentPlayerFactory()
{
}

QStringList SegmentPlayerFactory::mimeTypes() const
{
    return (QStringList() << QLatin1String(Constants::KINGVISION_SEGMENT_MIMETYPE));
}

Player::IPlayerPtr SegmentPlayerFactory::createPlayer() const
{
    Player::IPlayerPtr player;
    return player;
}

} // namespace Internal
} // namespace KingVision
