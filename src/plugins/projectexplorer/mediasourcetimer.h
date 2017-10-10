#ifndef MEDIASOURCE_TIMER_H
#define MEDIASOURCE_TIMER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace ProjectExplorer {
namespace Internal {

class MediaSourceTimer : public QObject
{
    Q_OBJECT

public:
    MediaSourceTimer();
    ~MediaSourceTimer();

    static MediaSourceTimer *instance();

    static void extensionsInitialized();

signals:
    void checkStatus();

private:
    QTimer *m_timer;
};

} // namespace Internal
} // namespace ProjectExplorer

#endif // MEDIASOURCE_TIMER_H
