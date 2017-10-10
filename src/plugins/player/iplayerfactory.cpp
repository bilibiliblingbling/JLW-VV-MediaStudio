#include "iplayerfactory.h"

namespace Player {

IPlayerFactory::IPlayerFactory(QObject *parent)
    : QObject(parent)
{
    setObjectName(QStringLiteral("ProjectExplorer.IPlayerFactory"));
}

IPlayerFactory::~IPlayerFactory()
{
}

/*!
 * \brief mimeTypes
 * \return mimetypes which are supported by player create by \c createPlayer()
 */

/*!
 * \brief createPlayer
 * \return a \c IPlayer
 */

} // namespace Player
