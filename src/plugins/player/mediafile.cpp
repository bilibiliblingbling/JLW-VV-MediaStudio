#include "mediafile.h"

namespace Player {

/*!
 *  player -> mediafile
 *                ^
 *                |
 *             segment
 *
 *  mediasource -> segment -> mediafile
 *
 */

MediaFile::MediaFile(QObject *parent)
    : QIODevice(parent)
{
}

MediaFile::~MediaFile()
{
}

} // namespace Player
