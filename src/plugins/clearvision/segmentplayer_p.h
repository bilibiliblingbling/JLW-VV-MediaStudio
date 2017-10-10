#ifndef CLEARVISION_SEGMENT_PLAYER_P_H
#define CLEARVISION_SEGMENT_PLAYER_P_H

#include <QMutex>
#include <QObject>


#include <projectexplorer/iqueryroutelistcallback.h>
#include <player/streamhelper.h>

#include "player/iscreen.h"


namespace ClearVision {
namespace Internal {
namespace PE = ProjectExplorer;
class Segment;

enum SegStatus
{
    SEG_S_OPEN,
    SEG_S_PLAY,
    SEG_S_PAUSE,
    SEG_S_STOP,
    SEG_S_CLOSE,
    SEG_S_END,
    SEG_S_EXPIRED,
    SEG_S_OPEN_ERROR,
    SEG_S_PLAY_ERROR,
};

enum SegPendStatus
{
    SEG_PS_NONE = 64,
    SEG_PS_OPENING,
    SEG_PS_PLAY_REQUESTING,
    SEG_PS_SEEKING,
};

enum
{
    RECV_END_FLAG_NONE,
    RECV_END_FLAG_END,
    RECV_END_FLAG_EXPIRED,
};

enum SegPlayMode
{
    SEG_PLAY_NORMAL,
    SEG_PLAY_SKIP_FORWARD,
    SEG_PLAY_SKIP_BACKWARD,
    SEG_PLAY_REVERSE,
};

class SegmentPlayerPrivate : public QObject, public StreamSDK::IClientCallback, public IStreamPlayCallback, public PE::IQueryRouteListCallback
{
    Q_OBJECT

public:
    explicit SegmentPlayerPrivate(QObject *parent = 0);
    ~SegmentPlayerPrivate();

public:
    void open(STREAM_OPEN_DATA*);
    void play();
    void pause();
    void stop();
    void doStop(BOOL bPlayEnd);
    void close();
    void doClose(BOOL bActive);
    void seek(quint64 offset);
    void skip (long intervalSec);
    quint64 getPlayTime();

public:
    Q_SIGNAL void reachEnd();

public:
    StreamSDK::RESULT openStream(const StreamSDK::RouteList& rl, StreamSDK::Stream::DIR dir, StreamSDK::IContext* ctxt);

    // StreamSDK::IClientCallback interface
public:
    void OnOpenStreamSuccess(StreamSDK::Stream stream, StreamSDK::IContext *ctxt, StreamSDK::Client &client);
    void OnOpenStreamError(StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client);
    void OnStreamData(StreamSDK::Stream stream, StreamSDK::StreamData data, StreamSDK::StreamSuspender &suspender, StreamSDK::Client &client);
    void OnStreamClosed(StreamSDK::Stream stream, StreamSDK::CLOSE_REASON reason, StreamSDK::Client &client);
    void OnStreamSendable(StreamSDK::Stream stream, StreamSDK::Client &client);
    void OnStreamCommand(StreamSDK::Stream stream, StreamSDK::Command cmd, StreamSDK::Client &client);
    void OnStreamCommandResponse(StreamSDK::Stream stream, StreamSDK::CommandResponse response, StreamSDK::Client &client);
    void OnStreamNotify(StreamSDK::Stream stream, StreamSDK::Notification notification, StreamSDK::Client &client);
    void OnAddDynSndDstResult(StreamSDK::Stream stream, const StreamSDK::StreamUrl &url, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client);
    void OnRemoveDynSndDstResult(StreamSDK::Stream stream, const StreamSDK::StreamUrl &url, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client);
    void OnClearDynSndDstResult(StreamSDK::Stream stream, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client);
    void OnEnableDynSndBroadcastResult(StreamSDK::Stream stream, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client);
    void OnDisableDynSndBroadcastResult(StreamSDK::Stream stream, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client);
    void OnDynSndDstEliminated(StreamSDK::Stream stream, const StreamSDK::StreamUrl &url, StreamSDK::CLOSE_REASON reason, StreamSDK::Client &client);
signals:
    void streamEnd();

    void bufferAvailable(quint32 dwStreamType);


public:
    long SetCallback(IStreamPlayCallback *pCallback);
    long SetCallbackFuncs(DWORD dwFuncs);

public:
    bool inPutStreamData(StreamSDK::StreamData data);
    bool inPutStreamFlag(DWORD dwFlag);
    // IStreamPlayCallback interface
public:
    HRESULT OnBufferAvailable(DWORD dwStreamType);
    HRESULT OnStreamFlag(const STREAM_INFO *pStreamInfo, LONG lHandled, LONG *plStatus);
    HRESULT OnAudioDecode(const AUDIO_FRAME *pFrame);
    HRESULT OnVideoDecode(const VIDEO_FRAME *pFrame);
    HRESULT BeforeVideoRender(IVideoFrame *pFrame);

public:
    void onQueryRouteListSuccess(PE::Segment *segment, const StreamSDK::RouteList &rl);
    void onQueryRouteListFailed(PE::Segment *segment);

public:
    IStreamPlayPtr m_player;
    SegStatus m_status;
    SegPlayMode m_playMode;

public:
    StreamSDK::Stream m_stream;
    SegPendStatus m_pendStatus;
    mutable QMutex m_lock;

    bool m_bPutable;
    int m_recvEndFlag;
    bool m_bBufferSendEnabled;

public:
    STREAM_OPEN_DATA m_openData;

    StreamSDK::StreamData m_remainData;
    DWORD m_dwRemainFlag;
    StreamSDK::Command m_pendingCommand;

public:
    quint64 m_ullBgnTime;
    quint64 m_ullEndTime;
    quint64 m_ullPlayTime;

private slots:
    void streamPlayEnd();
    void streamBufferAvailable(quint32 dwStreamType);


};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_SEGMENT_PLAYER_P_H
