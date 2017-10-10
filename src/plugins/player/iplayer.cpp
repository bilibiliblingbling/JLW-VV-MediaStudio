#include "iplayer.h"

namespace Player {

/*!
 *  \class IPlayer
 *  \brief The IPlayer class
 *
 *  播放器接口, TODO: complement required
 *
 */

IPlayer::IPlayer(QObject *parent)
    : QObject(parent)
{
}

IPlayer::~IPlayer()
{
}

ProjectExplorer::MetaData *IPlayer::metaData()
{
    return 0;
}

void IPlayer::aboutToPlayMetaData(ProjectExplorer::MetaData *metadata)
{
    if (metadata && metadata->isValid())
        aboutToPlayMetaDataImpl(metadata);
}

void IPlayer::setScreen(IScreen *screen)
{
    // TODO
    m_screen = screen;
}

IScreen *IPlayer::getScreen()
{
    // TODO
    return m_screen;
}

bool IPlayer::canRecord() const
{
    return false;
}

bool IPlayer::isRecording() const
{
    return false;
}

void IPlayer::setRecording(bool)
{
}

bool IPlayer::isCyclic() const
{
    return false;
}

void IPlayer::setCyclic(bool)
{
}

quint64 IPlayer::currentTime() const
{
    return 0;
}

quint64 IPlayer::beginTime() const
{
    return 0;
}

quint64 IPlayer::endTime() const
{
    return 0;
}

void IPlayer::seek(quint64)
{
}

void IPlayer::skip(qint32)
{
}

double IPlayer::speedRate() const
{
    return 0.;
}

void IPlayer::setSpeedRate(double)
{
}

void IPlayer::stepFrame()
{
    stepFrame(1);
}

void IPlayer::stepFrame(qint32)
{
}

} // namespace Player
