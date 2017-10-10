#ifndef IPLAYER_FACTORY_H
#define IPLAYER_FACTORY_H

#include <QObject>

#include "iplayer.h"
#include "player_global.h"

namespace Player {

class PLAYER_EXPORT IPlayerFactory : public QObject
{
    Q_OBJECT

public:
    explicit IPlayerFactory(QObject *parent = 0);
    virtual ~IPlayerFactory();

public:
    virtual QStringList mimeTypes() const = 0;
    virtual IPlayerPtr createPlayer() const = 0;
};

} // namespace Player

#endif // IPLAYER_FACTORY_H
