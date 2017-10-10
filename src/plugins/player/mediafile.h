#ifndef MEDIA_FILE_H
#define MEDIA_FILE_H

#include "player_global.h"

#include <QIODevice>

namespace Player {

class PLAYER_EXPORT MediaFile : public QIODevice
{
    Q_OBJECT

public:
    explicit MediaFile(QObject *parent = 0);
    virtual ~MediaFile();


};

} // namespace Player

#endif // MEDIA_FILE_H
