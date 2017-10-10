#ifndef KINGVISION_PLAYER_FACTORY_H
#define KINGVISION_PLAYER_FACTORY_H

#include <player/iplayerfactory.h>

namespace KingVision {
namespace Internal {

class PlayerFactory : public Player::IPlayerFactory
{
    Q_OBJECT

public:
    explicit PlayerFactory(QObject *parent = 0);
    ~PlayerFactory();

public:
    QStringList mimeTypes() const Q_DECL_OVERRIDE;
    Player::IPlayerPtr createPlayer() const Q_DECL_OVERRIDE;
};


class CameraPlayerFactory : public Player::IPlayerFactory
{
    Q_OBJECT

public:
    explicit CameraPlayerFactory(QObject *parent = 0);
    ~CameraPlayerFactory();

public:
    QStringList mimeTypes() const Q_DECL_OVERRIDE;
    Player::IPlayerPtr createPlayer() const Q_DECL_OVERRIDE;
};


class SegmentPlayerFactory : public Player::IPlayerFactory
{
    Q_OBJECT

public:
    explicit SegmentPlayerFactory(QObject *parent = 0);
    ~SegmentPlayerFactory();

public:
    QStringList mimeTypes() const Q_DECL_OVERRIDE;
    Player::IPlayerPtr createPlayer() const Q_DECL_OVERRIDE;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_PLAYER_FACTORY_H
