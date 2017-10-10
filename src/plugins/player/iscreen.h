#ifndef PLAYER_ISCREEN_H
#define PLAYER_ISCREEN_H

#include <StreamPlay/StreamPlay2_GDef.h>

#include "player_global.h"

namespace Player {

class PLAYER_EXPORT IScreen
{
public:
    virtual ~IScreen() {}

public:
    virtual HWND getWinId() const = 0;

    virtual void showVideo() = 0;
    virtual void showBackGround() = 0;
    virtual void showMessage(const QString &msg) = 0;
};

} // namespace Player

#endif // PLAYER_ISCREEN_H
