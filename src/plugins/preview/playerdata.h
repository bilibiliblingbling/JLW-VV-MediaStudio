#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <QObject>

#include "player/iplayer.h"

namespace Preview {
namespace Internal {

QString seconds_to_DHMS(quint64 duration);

class IPlayer;
class PlayerData;
typedef QSharedPointer<PlayerData> PlayerDataPtr;

class PlayerData
{
public:
    PlayerData();
    virtual ~PlayerData();

public:
    virtual void setData();
    virtual quint64 getBgnTime() const;

public:
    void setPlayer(const Player::IPlayerPtr &player);
    void setTimeIn(quint64);
    void setTimeOut(quint64);

    void setPlayerInGroup(bool);

public:
    void clearTimeIn();
    void clearTimeOut();
    void clearTimeInOut();
    void resetPlayer();

public:
    Player::IPlayerPtr getPlayer();
    quint64 getTimeOut() const;
    quint64 getTimeIn() const;

    bool playerInGroup() const;

protected:
    Player::IPlayerPtr m_player;

    quint64 m_timeIn;
    quint64 m_timeOut;
    bool m_isPlayerInGroup;
};


class PlayBackData : public PlayerData
{
public:
    PlayBackData();
    ~PlayBackData();

public:
    void setData();
    quint64 getBgnTime() const;

public:
    quint64 getEndTime() const;

    quint64 getTimeRange() const;

private:
    quint64 m_bgnTime;
    quint64 m_endTime;
};


class LivePlayerData : public PlayerData
{
public:
    LivePlayerData();
    ~LivePlayerData();

public:
    void setData();
    quint64 getBgnTime() const;

private:
    quint64 m_bgnPlayTime;
};


/*
class PlayerData
{
public:
    Player::IPlayerPtr m_player;

    quint64 m_bgnTime;
    quint64 m_endTime;

    quint64 m_timeRange;

    quint64 m_timeIn;
    quint64 m_timeOut;

    bool isPlayerInGroup;
};

*/

} // namespace Internal
} // namespace Preview

#endif // PLAYER_DATA_H
