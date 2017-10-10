#include "playerdata.h"

namespace Preview {
namespace Internal {

PlayerData::PlayerData()
{
}

PlayerData::~PlayerData()
{
}

void PlayerData::setData()
{
    m_timeIn = 0;
    m_timeOut = 0;
    m_isPlayerInGroup = false;
}

quint64 PlayerData::getBgnTime() const
{
    return 0;
}

void PlayerData::setPlayer(const Player::IPlayerPtr &player)
{
    if (!player)
        return;

    m_player = player;
}

void PlayerData::setTimeIn(quint64 timeIn)
{
    m_timeIn = timeIn;
}

void PlayerData::setTimeOut(quint64 timeOut)
{
    m_timeOut = timeOut;
}

void PlayerData::setPlayerInGroup(bool isInGroup)
{
    m_isPlayerInGroup = isInGroup;
}

void PlayerData::clearTimeIn()
{
    m_timeIn = 0;
}

void PlayerData::clearTimeOut()
{
    m_timeOut = 0;
}

void PlayerData::clearTimeInOut()
{
    m_timeIn = 0;
    m_timeOut = 0;
}

void PlayerData::resetPlayer()
{
    if (m_player)
        m_player.reset();
}

Player::IPlayerPtr PlayerData::getPlayer()
{
    return m_player;
}

quint64 PlayerData::getTimeOut() const
{
    return m_timeOut;
}

quint64 PlayerData::getTimeIn() const
{
    return m_timeIn;
}

bool PlayerData::playerInGroup() const
{
    return m_isPlayerInGroup;
}

///////////////////////////////////////////////////////////////////////////////
PlayBackData::PlayBackData()
{

}

PlayBackData::~PlayBackData()
{

}

void PlayBackData::setData()
{
    if (m_player)
    {
        m_bgnTime = m_player->beginTime();
        m_endTime = m_player->endTime();
    }
    else
    {
        m_bgnTime = 0;
        m_endTime = 0;
    }

    m_timeIn = 0;
    m_timeOut = 0;
    m_isPlayerInGroup = false;
}

quint64 PlayBackData::getBgnTime() const
{
    return m_bgnTime;
}

quint64 PlayBackData::getEndTime() const
{
    return m_endTime;
}

quint64 PlayBackData::getTimeRange() const
{
    return (m_endTime - m_bgnTime);
}

/////////////////////////////////////////////////////////////////////////////
LivePlayerData::LivePlayerData()
{

}

LivePlayerData::~LivePlayerData()
{

}

void LivePlayerData::setData()
{
    //TODO; about player
    m_bgnPlayTime = 0;

    m_timeIn = 0;
    m_timeOut = 0;
    m_isPlayerInGroup = false;
}

quint64 LivePlayerData::getBgnTime() const
{
    return m_bgnPlayTime;
}

} // namespace Internal
} // namespace Preview


