#ifndef KV_MS_UTILS_SKIN_BUTTON_H
#define KV_MS_UTILS_SKIN_BUTTON_H

#include <QPushButton>

#include "utils_global.h"

namespace Utils {

class MEDIASTUDIO_UTILS_EXPORT SkinButton : public QPushButton
{
    Q_OBJECT

public:
    typedef enum {
        ButtonNormal        = 0,
        ButtonHovered       = 1,
        ButtonPressed       = 2,
        ButtonDisabled      = 3
    } ButtonState;

public:
    SkinButton(qint32 type, QWidget *parent = 0);

public slots:
    void setIndex(bool checked);
    void setIndex(qint8 idx);

protected:
    QSize sizeHint() const;

    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void paintEvent(QPaintEvent *);

private:
    QPixmap getStatePixmap() const;

private:
    qint32 m_type;

    qint8 m_index;
    ButtonState m_state;
};

} // namespace Utils

#endif // KV_MS_UTILS_SKIN_BUTTON_H
