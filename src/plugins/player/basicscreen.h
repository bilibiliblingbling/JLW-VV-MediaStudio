#include <QWidget>

#include "player_global.h"
#include "iscreen.h"

namespace Player {

namespace Internal { class BasicScreenPrivate; }

class PLAYER_EXPORT BasicScreen : public QWidget, public IScreen
{
    Q_OBJECT

public:
    BasicScreen(QWidget *parent = 0);

public:
    HWND getWinId() const Q_DECL_OVERRIDE;

    // IScreen interface
public:
    void showBackGround() Q_DECL_OVERRIDE;
    void showMessage(const QString &msg) Q_DECL_OVERRIDE;
    void showVideo() Q_DECL_OVERRIDE;

private:
    Internal::BasicScreenPrivate *d;///X3...分别打印错误信息,background,图像 _p中保存
};

} // Namespace Player
