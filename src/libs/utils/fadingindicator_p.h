#ifndef FADING_INDICATOR_P_H
#define FADING_INDICATOR_P_H

#include "fadingindicator.h"

QT_BEGIN_NAMESPACE
class QGraphicsOpacityEffect;
class QLabel;
class QPixmap;
QT_END_NAMESPACE

namespace Utils {
namespace Internal {

class FadingIndicatorPrivate : public QWidget
{
    Q_OBJECT

public:
    FadingIndicatorPrivate(QWidget *parent, FadingIndicator::TextSize size);

    void setText(const QString &text);

    void setPixmap(const QString &uri);

    void run(int ms);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void runInternal();

private:
    QGraphicsOpacityEffect *m_effect;
    QLabel *m_label;
    QPixmap m_pixmap;
};


} // namespace Internal
} // namespace Utils

#endif // FADING_INDICATOR_P_H
