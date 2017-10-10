#ifndef KINGVISION_SEGMENT_PLAYER_P_H
#define KINGVISION_SEGMENT_PLAYER_P_H

#include <QObject>
#include <QWidget>

#include <StreamPlay/StreamPlay2_G.h>


namespace KingVision {
namespace Internal {

class SegmentPlayerPrivate : public QObject, public IStreamPlayCallback
{
    Q_OBJECT

public:
    explicit SegmentPlayerPrivate(QObject *parent = 0);
    ~SegmentPlayerPrivate();

public:
    void setScreen(QWidget *screen);
    void screenDestroied(QObject *screen);

public:
    void play();

    // IStreamPlayCallback interface
public:
    HRESULT OnBufferAvailable(DWORD dwStreamType);
    HRESULT OnStreamFlag(const STREAM_INFO *pStreamInfo, LONG lHandled, LONG *plStatus);
    HRESULT OnAudioDecode(const AUDIO_FRAME *pFrame);
    HRESULT OnVideoDecode(const VIDEO_FRAME *pFrame);
    HRESULT BeforeVideoRender(IVideoFrame *pFrame);

public:
    bool m_cyclic;
    double m_speedRate;

public:
    QWidget *m_screen;

public:
    IStreamPlayPtr m_player;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_SEGMENT_PLAYER_P_H
