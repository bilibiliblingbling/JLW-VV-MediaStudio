#ifndef CLEARVISION_PLAYER_FACTORY_H
#define CLEARVISION_PLAYER_FACTORY_H

#include <player/iplayerfactory.h>

namespace ClearVision {
namespace Internal {

class PlayerFactory : public Player::IPlayerFactory
{
    Q_OBJECT

public:
    explicit PlayerFactory(QObject *parent = 0);
    ~PlayerFactory();

public:
    QStringList mimeTypes() const;
    Player::IPlayerPtr createPlayer() const;
};

} // namespace Internal
} // namespace ClearVision


#endif // CLEARVISION_PLAYER_FACTORY_H
