#ifndef BASICSCREEN_P_H
#define BASICSCREEN_P_H

#include <QStackedWidget>

#include "iscreen.h"

class QLabel;

namespace Player {
namespace Internal {

/*! \brief 播放组件 */
class RealVideo : public QWidget
{
    Q_OBJECT

public:
    RealVideo(QWidget* parent = 0)
        :QWidget(parent){}

    QPaintEngine *paintEngine() const
    {
        return NULL;
    }
};

/*! @brief 消息窗口 */
class MessageWidget : public QFrame
{
    Q_OBJECT

public:
    MessageWidget();

    void showMessage(const QString& msg);

private:
    QLabel* m_message;
};

/*!
 * @brief 背景窗口
 * BackGroundWidget可拓展为
 * 设置背景图或者根据不同模式更换不同背景图片
 */

class BackGroundWidget : public QFrame
{
    Q_OBJECT

public:
    BackGroundWidget();
#if 0
    void setVodMode(bool bVod);
protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    QLabel* m_pic;
    bool m_bVod;
#endif
};

/*! @brief 播放组件的父窗口 */
class VideoWidget : public QFrame
{
    Q_OBJECT

public:
    VideoWidget();

    HWND getRealPlayWindowId() const;

private:
    RealVideo *m_realvideo;
};

/*! @brief 负责显示播放相关信息 */
class HeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeaderWidget(QWidget* parent = 0);

public:
    void setName(const QString& name);
    void setTime(qulonglong time);
    void hideTime();

private:
    QLabel* m_nameLabel;
    QLabel* m_timeLabel;
};

class BasicScreenPrivate
{
public:
    QStackedWidget *m_stack;

    MessageWidget *m_message;
    VideoWidget *m_video;
    BackGroundWidget *m_background;
};

}
} // namespace Player

#endif // BASICSCREEN_P_H
