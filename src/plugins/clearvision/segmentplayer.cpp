#include <QDebug>

#include <utils/tmplog.h>
#include <utils/communicator/udocacontext.h>
#include <projectexplorer/mediasource.h>
#include <projectexplorer/iqueryroutelistcallback.h>

#include <storage/storage_format.h>
#include <storage/storage_def.h>

#include "constants.h"
#include "mediasource.h"
#include "segment.h"
#include "segmentplayer.h"
#include "segmentplayer_p.h"


namespace ClearVision {
namespace Internal {

SegmentPlayer::SegmentPlayer(QObject *parent)
    : Player::IPlayer(parent)
    , d(new SegmentPlayerPrivate(this))
{
    d->m_status = SEG_S_CLOSE;
    connect(d,SIGNAL(reachEnd()),this,SIGNAL(reachEnd()));
}

SegmentPlayer::~SegmentPlayer()
{

}

bool SegmentPlayer::isValid() const
{
    return d->m_player;
}

QStringList SegmentPlayer::mimeTypes() const
{
    return (QStringList() << QLatin1String(Constants::CLEARVISION_SEGMENT_MIMETYPE));
}

Player::IPlayer::PlayMode SegmentPlayer::playMode() const
{
    return PlaybackMode;
}

bool SegmentPlayer::isOpened() const
{
    d->m_stream.IsValid();
}

bool SegmentPlayer::isPlaying() const
{
    return ((d->m_status == SEG_S_PLAY)
            || (d->m_status == SEG_S_OPEN));
}

bool SegmentPlayer::isPaused() const
{
    return (d->m_status == SEG_S_PAUSE);
}

bool SegmentPlayer::isStopped() const
{
    return (d->m_status == SEG_S_STOP);
}

void SegmentPlayer::play()
{
    QMutexLocker lock(&(d->m_lock));
    if ((d->m_status) == SEG_S_OPEN
            || (d->m_status) == SEG_S_PLAY) {
        d->pause();
        //updatePlayButton(false);
    } else {

        if (d->m_status == SEG_S_CLOSE) {
            playRecord(m_metadata);
        } else {
            d->play();
            //updatePlayButton(true);
        }
    }
}

void SegmentPlayer::pause()
{
    QMutexLocker lock(&(d->m_lock));
    d->pause();
}

void SegmentPlayer::stop()
{
    //QMutexLocker lock(&(d->m_lock));
    TM_DBG<<"stop*****stop";
    d->stop();
}

void SegmentPlayer::close()
{
    d->close();
}

void SegmentPlayer::seek(quint64 offset)
{
    d->seek(offset);
}

void SegmentPlayer::skip(quint32 intervalSec)
{
    d->skip(intervalSec);
}


quint64 SegmentPlayer::beginTime() const
{
    return d->m_ullBgnTime;
}

quint64 SegmentPlayer::endTime() const
{
    return d->m_ullEndTime;
}


bool SegmentPlayer::isCyclic() const
{

}

void SegmentPlayer::setCyclic(bool b)
{

}

double SegmentPlayer::speedRate() const
{
    double rate = 1.0;
    if (d->m_player) {
        d->m_player->GetRate(&rate);
    }

    return rate;
}

void SegmentPlayer::setSpeedRate(double rate)
{
    if (!(d->m_player)) {
        return;
    }
    d->m_player->SetRate(rate);
}

void SegmentPlayer::stepFrames(qint32 frames)
{

}

void SegmentPlayer::timerEvent(QTimerEvent *e)
{
    if (isPlaying()) {
        quint64 elapseTime = 0;
        if (d->getPlayTime()) {
            elapseTime = d->getPlayTime() - d->m_ullBgnTime;
        } else {
            elapseTime = 0;
        }
        emit elapse(elapseTime);
    } else {
        return;
    }
}

quint64 SegmentPlayer::currentTime() const
{
    return d->getPlayTime();
}

void SegmentPlayer::playRecord(ProjectExplorer::MetaData *metadata)
{
    d->doStop(true);
    d->m_status = SEG_S_OPEN;
    PE::MediaSource::queryRouteList(qobject_cast<ProjectExplorer::Segment *>(metadata),d);
    d->m_openData.hParentWnd = (HWND)(getScreen()->getWinId());
    d->m_ullBgnTime = qobject_cast<ProjectExplorer::Segment *>(metadata)->bgnTime();
    //TM_DBG<<"HHHHH time HHHHH"<<d->m_ullBgnTime;
    d->m_ullEndTime = qobject_cast<ProjectExplorer::Segment *>(metadata)->endTime();
    startTimer(1000);
}

void SegmentPlayer::aboutToPlayMetaDataImpl(ProjectExplorer::MetaData *metadata)
{
    m_metadata = metadata;
    playRecord(metadata);
}


////////////////////////////////////////////////////////////////////////////////


SegmentPlayerPrivate::SegmentPlayerPrivate(QObject *parent)
    : QObject(parent)
{
    m_ullBgnTime = 0;
    m_ullEndTime = 0;
    m_ullPlayTime = 0;
    QMetaObject::Connection con;
    con = connect(this,SIGNAL(streamEnd()),this,SLOT(streamPlayEnd()));
    Q_ASSERT(con);
    con = connect(this,SIGNAL(bufferAvailable(quint32)),this,SLOT(streamBufferAvailable(quint32)));
    Q_ASSERT(con);
}

SegmentPlayerPrivate::~SegmentPlayerPrivate()
{

}

void SegmentPlayerPrivate::open(STREAM_OPEN_DATA *)
{
    if (!m_player) {
        m_player = CreateStreamPlay();
        if (m_player) {
            m_player->SetCallback(this);
            m_player->SetCallbackFuncs(STREAMPLAY_CALLBACK_FUNC_BUFFER_AVAILABLE
                                       | STREAMPLAY_CALLBACK_FUNC_STREAM_FLAG
                                       | STREAMPLAY_CALLBACK_FUNC_BEFORE_VIDEO_RENDER);
        } else {
            TM_DBG<<"error"<<endl;
            return;
        }
    }

    //m_openData.hParentWnd = ;
    m_openData.hNotifyWnd = NULL;
    m_openData.dwPlayMode = PLAY_MODE_PLAYBACK;
    m_openData.bMixedStream = true;
    m_openData.dwVideoAlgo = VIDEO_ALGO_UNKNOWN;
    m_openData.dwAudioAlgo = AUDIO_ALGO_UNKNOWN;

    HRESULT res = m_player->Open(&m_openData);
    if (res == S_OK) {
        m_bPutable = true;
    }

    m_player->SetBufferedTime(200, 500);
    m_player->Play();
    m_player->SetRate(1.0);

    play();
}

void SegmentPlayerPrivate::play()
{
    if (!m_stream.IsValid()) {
        return;
    }

    if (m_pendingCommand.IsValid()) {
        if(m_pendingCommand.GetType() == StreamSDK::COMMAND_PLAY) {
            return;
        }
    } else {
        if (m_status == SEG_S_PLAY) {
            return;
        }
    }

    if (m_status == SEG_S_END || m_status == SEG_S_EXPIRED) {
        return;
    }
    bool bResumePlay = false;

    if (m_status == SEG_S_PAUSE) {
        if (m_player) {
            m_player->Play();
        }
        m_status = SEG_S_PLAY;
        bResumePlay = true;
    }

    StreamSDK::Command playCmd = StreamSDK::Command::Create(StreamSDK::COMMAND_PLAY);
    m_stream.SendCommand(playCmd);

    if (!bResumePlay) {
        m_pendingCommand = playCmd;
        m_pendStatus = SEG_PS_PLAY_REQUESTING;
    }

}

void SegmentPlayerPrivate::pause()
{
    //QMutexLocker lock(&m_lock);
    if (!m_stream.IsValid()) {
        return;
    }
    if (m_status != SEG_S_PLAY) {
        return;
    }

    StreamSDK::Command playCmd = StreamSDK::Command::Create(StreamSDK::COMMAND_PAUSE);
    m_stream.SendCommand(playCmd);

    if (m_player) {
        m_player->Pause();
    }

    m_status = SEG_S_PAUSE;
}

void SegmentPlayerPrivate::close()
{
//    QMutexLocker lock(&m_lock);
    if (!m_stream.IsValid()) {
        return;
    }
    if (m_status == SEG_S_STOP) {
        return;
    }
//    lock.unlock();
    doClose(true);
//    lock.relock();

    m_status = SEG_S_STOP;
}

void SegmentPlayerPrivate::doClose(BOOL bPlayEnd)
{
    m_stream.SendCommand(StreamSDK::Command::Create(StreamSDK::COMMAND_STOP));
    if (m_player) {
        if (!bPlayEnd) {
            m_player->Stop();
            m_player->ClearRecentVideo();
            m_player->SetFastPlaybackFrameRate(0);
            m_player->SetReverseMode(false);
            m_player->StopRecord();
            m_player->StopRecordUserData();
            m_player->StopCapture();
        }
    }

    m_pendingCommand = StreamSDK::Command();
    m_stream.ResumeReceive();

    m_remainData = StreamSDK::StreamData();
    m_dwRemainFlag = STREAM_FLAG_NONE;
    m_ullPlayTime = 0;
    m_recvEndFlag = RECV_END_FLAG_NONE;

}

void SegmentPlayerPrivate::stop()
{
    //QMutexLocker lock(&m_lock);
    //lock.unlock();
    doStop(true);
    //lock.relock();
    m_status = SEG_S_CLOSE;
}

void SegmentPlayerPrivate::doStop(BOOL bActive)
{
    if (m_stream.IsValid()) {
        if (bActive) {
            m_stream.Close(StreamSDK::CLOSE_REASON_USER);
        }
        m_stream = StreamSDK::Stream();
    }

    if (m_player) {
        m_player->Stop();
        m_player->ClearRecentVideo();
        m_player->SetFastPlaybackFrameRate(0);
        m_player->SetReverseMode(false);
        m_player->SetRate(1.0);

        m_player->SetMute(false);
        m_player->StopCapture();
        m_player->StopRecord();
        m_bPutable = true;
    }

    m_pendingCommand = StreamSDK::Command();
    m_remainData = StreamSDK::StreamData();
    m_dwRemainFlag = STREAM_FLAG_NONE;
    m_ullBgnTime = 0;
    m_ullEndTime = 0;
    m_ullPlayTime = 0;
}

void SegmentPlayerPrivate::seek(quint64 offset)
{
    if (!m_stream.IsValid()){
        return;
    }
    if (m_status != SEG_S_PLAY&&m_status != SEG_S_PAUSE){
        return;
    }

    StreamSDK::COMMAND_TYPE pendingCmdType = m_pendingCommand.GetType();
    if (m_pendingCommand.IsValid() && pendingCmdType != StreamSDK::COMMAND_SEEK){
        if (m_pendingCommand.IsValid()) {
            return;
        }
        return;
    }

    m_pendingCommand = StreamSDK::Command::Create(StreamSDK::COMMAND_SEEK, 1);
    m_pendingCommand.SetArgu(0, offset);

    m_stream.SendCommand(m_pendingCommand);

    m_stream.ResumeReceive();
    m_remainData = StreamSDK::StreamData();

}

void SegmentPlayerPrivate::skip(long intervalSec)
{
    if (!m_stream.IsValid()) {
        return;
    }

    if (m_status != SEG_S_PLAY) {
        return;
    }

    if (intervalSec == 0) {
        m_pendingCommand = StreamSDK::Command::Create(StreamSDK::COMMAND_PLAY,1);
        if (m_player) {
            m_player->Pause();
        }

    } else if (intervalSec > 0) {
        m_pendingCommand = StreamSDK::Command::Create(StreamSDK::COMMAND_SKIP_FORWARD, 2);
        m_pendingCommand.SetArgu(1, intervalSec);
    } else {
        m_pendingCommand = StreamSDK::Command::Create(StreamSDK::COMMAND_SKIP_BACKWARD, 2);
        m_pendingCommand.SetArgu(1, -intervalSec);
    }

    quint64 ullOffset = getPlayTime();
    //TM_DBG<<"AA"<<getPlayTime()<<m_ullBgnTime;
    if (ullOffset > m_ullBgnTime) {
        ullOffset -= m_ullBgnTime;
    } else {
        ullOffset = 0;
    }

    m_pendingCommand.SetArgu(0, ullOffset);
    m_stream.SendCommand(m_pendingCommand);

    m_stream.ResumeReceive();
    m_bPutable = true;
    m_remainData = StreamSDK::StreamData();
    m_dwRemainFlag = STREAM_FLAG_NONE;
    m_pendStatus = SEG_PS_SEEKING;
}

quint64 SegmentPlayerPrivate::getPlayTime()
{
    if (!m_player) {
        return 0;
    }

    if (m_status == SEG_S_END || m_status == SEG_S_EXPIRED) {
        return 0;
    }
    quint64 ullTime = 0;
    m_player->GetPosition((int64*)&ullTime);
    //TM_DBG<<"ullTime:"<<ullTime;
    ullTime /= 1000;
    if (ullTime == 0) {
        return m_ullPlayTime;
    }
    m_ullPlayTime = ullTime;

    return m_ullPlayTime;
}

StreamSDK::RESULT SegmentPlayerPrivate::openStream(const StreamSDK::RouteList &rl, StreamSDK::Stream::DIR dir, StreamSDK::IContext *ctxt)
{
    StreamSDK::Client* sc = new StreamSDK::Client();
    *sc = StreamSDK::Client::Create(*this);

    StreamSDK::RESULT res = sc->OpenStream(rl, StreamSDK::Stream::DIR_S2C, ctxt);

    TM_DBG<<"res"<<res;
    return res;

}

void SegmentPlayerPrivate::OnOpenStreamSuccess(StreamSDK::Stream stream, StreamSDK::IContext *ctxt, StreamSDK::Client &client)
{
    m_stream = stream;
    STREAM_OPEN_DATA open_data;
    open(&open_data);
}

void SegmentPlayerPrivate::OnOpenStreamError(StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client)
{
    close();
}

void SegmentPlayerPrivate::OnStreamData(StreamSDK::Stream stream, StreamSDK::StreamData data, StreamSDK::StreamSuspender &suspender, StreamSDK::Client &client)
{
    //TM_DBG<<"data";
    if (!m_player) {
        return;
    }

    if (m_stream != stream) {
        return;
    }

    if (m_status != SEG_S_PLAY ) {
        return;
    }

    if (m_pendStatus == SEG_PS_SEEKING) {
        return;
    }

    if (!inPutStreamData(data)) {
        //TM_DBG<<"suspend";
        suspender.Suspend();
    }
}

void SegmentPlayerPrivate::OnStreamClosed(StreamSDK::Stream stream, StreamSDK::CLOSE_REASON reason, StreamSDK::Client &client)
{
    close();
}

void SegmentPlayerPrivate::OnStreamSendable(StreamSDK::Stream stream, StreamSDK::Client &client)
{

}

void SegmentPlayerPrivate::OnStreamCommand(StreamSDK::Stream stream, StreamSDK::Command cmd, StreamSDK::Client &client)
{

}

void SegmentPlayerPrivate::OnStreamCommandResponse(StreamSDK::Stream stream, StreamSDK::CommandResponse response, StreamSDK::Client &client)
{

    TM_DBG << "command reponsonse type:" << response.GetCommand().GetType();
    TM_DBG << "command response result:" << response.GetResult();
    if (stream != m_stream) {
        return;
    }
    StreamSDK::Command cmd = response.GetCommand();
    if (cmd != m_pendingCommand) {
        return;
    }

    StreamSDK::COMMAND_TYPE cmdType = m_pendingCommand.GetType();

    if (cmdType == StreamSDK::COMMAND_PLAY) {
        if (response.GetResult() == StreamSDK::RSLT_S_OK) {
            if (m_pendStatus == SEG_PS_SEEKING) {
                if (m_player) {
                    m_player->SetFastPlaybackFrameRate(0);
                    m_player->SetReverseMode(false);
                }
            }

            {
                if (m_player) {
                    m_player->Play();
                }
                if (m_status != SEG_S_PAUSE) {
                    m_status = SEG_S_PLAY;
                }
            }

            m_ullPlayTime = m_ullBgnTime;
            //TM_DBG<<"m_ullPlayTime:"<<m_ullPlayTime;
            if (response.GetArguCount() > 0) {
                m_ullPlayTime += response.GetArgu(0);
            }

            m_status = SEG_S_PLAY;

        } else {
            m_status = SEG_S_PLAY_ERROR;
        }
    } else if (cmdType == StreamSDK::COMMAND_PAUSE) {

        if (response.GetResult() == StreamSDK::RSLT_S_OK) {

            if (m_status != SEG_S_PAUSE) {
                m_player->Pause();
                m_status = SEG_S_PAUSE;
            }

            if (m_pendStatus == SEG_PS_SEEKING) {
                if (m_player) {
                    m_player->SetFastPlaybackFrameRate(0);
                    m_player->SetReverseMode(false);
                }
            }

            m_status = SEG_S_PAUSE;
        } else {
            m_status = SEG_S_PLAY_ERROR;
        }
    } else if(cmdType == StreamSDK::COMMAND_SKIP_FORWARD ||
              cmdType == StreamSDK::COMMAND_SKIP_BACKWARD) {
        if (response.GetResult() == StreamSDK::RSLT_S_OK) {
            if (m_player) {
                m_player->SetFastPlaybackFrameRate(1.0);
            }
        }
        if(response.GetArguCount() > 0) {
            m_ullPlayTime = m_ullBgnTime + response.GetArgu(0);
        }

    } else if(cmdType == StreamSDK::COMMAND_SEEK) {

        if (m_player) {
            m_player->Stop();

            if (m_status == SEG_S_PAUSE) {
                m_player->Pause();
            } else {
                m_player->Play();
            }
        }

        if (response.GetResult() == StreamSDK::RSLT_S_OK) {

            if(response.GetArguCount() > 0) {
                m_ullPlayTime = m_ullBgnTime + response.GetArgu(0);
            }

        } else {
            TM_DBG<<"send error";
        }

    }

    if (m_pendStatus == SEG_PS_SEEKING) {
        if (response.GetResult() == StreamSDK::RSLT_S_OK) {
            m_recvEndFlag = RECV_END_FLAG_NONE;
        } else {
            if (m_recvEndFlag != RECV_END_FLAG_NONE) {
                doStop(false);
                m_status = SEG_S_STOP;
            }
        }
    }

    m_pendingCommand = StreamSDK::Command();
    m_pendStatus = SEG_PS_NONE;
}


void SegmentPlayerPrivate::OnStreamNotify(StreamSDK::Stream stream, StreamSDK::Notification notification, StreamSDK::Client &client)
{
    if (stream != m_stream) {
        return;
    }

    if (notification.GetType() == StreamSDK::NOTIFY_END ||
            notification.GetType() == StreamSDK::NOTIFY_EXPIRED) {
        if (m_status != SEG_S_PLAY && m_status != SEG_S_PAUSE) {
            return;
        }

        if (m_recvEndFlag == RECV_END_FLAG_NONE) {
            m_recvEndFlag = (notification.GetType() == StreamSDK::NOTIFY_EXPIRED ?
                        RECV_END_FLAG_EXPIRED : RECV_END_FLAG_END);
        }

        if (m_player) {
            inPutStreamFlag(STREAM_FLAG_END);
        }
    } else if (notification.GetType() == StreamSDK::NOTIFY_REVERSE_INTERVAL) {
        if (m_status != SEG_S_PLAY && m_status != SEG_S_PAUSE) {
            return;
        }

        if (m_pendStatus == SEG_PS_SEEKING) {
            return;
        }

        if (m_player) {
            inPutStreamFlag(STREAM_FLAG_SEGMENT_END);
        }
    }
}

void SegmentPlayerPrivate::OnAddDynSndDstResult(StreamSDK::Stream stream, const StreamSDK::StreamUrl &url, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client)
{

}

void SegmentPlayerPrivate::OnRemoveDynSndDstResult(StreamSDK::Stream stream, const StreamSDK::StreamUrl &url, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client)
{

}

void SegmentPlayerPrivate::OnClearDynSndDstResult(StreamSDK::Stream stream, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client)
{

}

void SegmentPlayerPrivate::OnEnableDynSndBroadcastResult(StreamSDK::Stream stream, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client)
{

}

void SegmentPlayerPrivate::OnDisableDynSndBroadcastResult(StreamSDK::Stream stream, StreamSDK::RESULT result, StreamSDK::IContext *ctxt, StreamSDK::Client &client)
{

}

void SegmentPlayerPrivate::OnDynSndDstEliminated(StreamSDK::Stream stream, const StreamSDK::StreamUrl &url, StreamSDK::CLOSE_REASON reason, StreamSDK::Client &client)
{

}

long SegmentPlayerPrivate::SetCallback(IStreamPlayCallback *pCallback)
{
    return m_player->SetCallback(pCallback);
}

long SegmentPlayerPrivate::SetCallbackFuncs(DWORD dwFuncs)
{
    return m_player->SetCallbackFuncs(dwFuncs);
}

bool SegmentPlayerPrivate::inPutStreamData(StreamSDK::StreamData data)
{
    if (data.GetLength() <= sizeof(storage::fm_mediasliceinfo)) {
        return true;
    }

    const storage::fm_mediasliceinfo& slice = *(storage::pfm_mediasliceinfo)data.GetData();
    if (slice.slice_len != data.GetLength() - sizeof(storage::fm_mediasliceinfo)) {
        return true;
    }

    DWORD dwStreamType;
    if (slice.media_major_type == storage::media_major_type_video) {
        dwStreamType = STREAM_TYPE_VIDEO;
    } else if (slice.media_major_type == storage::media_major_type_audio) {
        dwStreamType = STREAM_TYPE_AUDIO;
    } else if (slice.media_major_type == storage::media_major_type_video_audio) {
        dwStreamType = STREAM_TYPE_VIDEO_AUDIO;
    } else {
        return true;
    }
    if (!m_bPutable) {
        m_remainData = data;
        return false;
    }
    BOOL bOpenDataChanged = false;

    STREAM_INFO info;
    info.dwStreamId = 1;
    info.dwPacketFormat = slice.slice_property;
    info.dwStreamType = dwStreamType;
    info.llTimestamp = slice_timestamp(slice);
    info.dwStreamFlag = STREAM_FLAG_NONE;
    info.dwVideoAlgo = VIDEO_ALGO_UNKNOWN;
    info.dwAudioAlgo = AUDIO_ALGO_UNKNOWN;

    if ((slice.media_major_type == STREAM_TYPE_VIDEO ||
         slice.media_major_type == STREAM_TYPE_AUDIO) && m_openData.bMixedStream) {
        m_openData.bMixedStream = false;
        bOpenDataChanged = true;
    } else if ((slice.media_major_type == STREAM_TYPE_MIXED ||
               slice.media_major_type == STREAM_TYPE_VIDEO_AUDIO) && !m_openData.bMixedStream) {
        m_openData.bMixedStream = true;
        bOpenDataChanged = true;
    }

    if ((slice.media_major_type == STREAM_TYPE_VIDEO || m_openData.bMixedStream) &&
            m_openData.dwVideoAlgo != slice.media_sub_type) {
        m_openData.dwVideoAlgo = slice.media_sub_type;
        bOpenDataChanged = true;
    }
    if (slice.media_major_type == STREAM_TYPE_AUDIO &&
            m_openData.dwAudioAlgo != slice.media_sub_type) {
        m_openData.dwAudioAlgo = slice.media_sub_type;
        bOpenDataChanged = true;
    }
    if (m_openData.bMixedStream && m_openData.dwAudioAlgo != slice.media_sub_type2) {
        m_openData.dwAudioAlgo = slice.media_sub_type2;
        bOpenDataChanged = true;
    }
    if (bOpenDataChanged) {
        m_player->SetStreamFormat(&m_openData);
    }

    HRESULT res = m_player->PutData(&info, data.GetData() + sizeof(storage::fm_mediasliceinfo), slice.slice_len);
    if (res == E_OUTOFMEMORY) {
        m_remainData = data;
        m_bPutable = false;
        return false;
    }

    BOOL bNewFrame = false;
    bNewFrame = bNewFrame
            || ((dwStreamType == STREAM_TYPE_VIDEO
                 || dwStreamType == STREAM_TYPE_VIDEO_AUDIO)
                && (slice_sub_frame_type(slice) == storage::frame_sub_type_complete
                    || slice_sub_frame_type(slice) == storage::frame_sub_type_incomplete_bgn));

    return true;
}

bool SegmentPlayerPrivate::inPutStreamFlag(DWORD dwFlag)
{

    if (!m_bPutable) {
        return false;
    }

    STREAM_INFO info;
    info.dwStreamType = STREAM_TYPE_VIDEO;
    info.dwStreamId = 1;
    info.dwStreamFlag = dwFlag;

    HRESULT res = m_player->PutData(&info, NULL, 0);
    if (res == E_OUTOFMEMORY) {
        m_dwRemainFlag = dwFlag;
        m_bPutable = false;
        return false;
    }

    return true;
}

HRESULT SegmentPlayerPrivate::OnBufferAvailable(DWORD dwStreamType)
{
    emit bufferAvailable(dwStreamType);
    return S_OK;
}

HRESULT SegmentPlayerPrivate::OnStreamFlag(const STREAM_INFO *pStreamInfo, LONG lHandled, LONG *plStatus)
{
    if (lHandled == SF_HANDLE_RENDERED && pStreamInfo) {
        if (pStreamInfo->dwStreamFlag == STREAM_FLAG_END) {
            emit streamEnd();
        }
    }
}

HRESULT SegmentPlayerPrivate::OnAudioDecode(const AUDIO_FRAME *pFrame)
{
    return S_OK;
}

HRESULT SegmentPlayerPrivate::OnVideoDecode(const VIDEO_FRAME *pFrame)
{
    return S_OK;
}

HRESULT SegmentPlayerPrivate::BeforeVideoRender(IVideoFrame *pFrame)
{

    return S_OK;
}

void SegmentPlayerPrivate::onQueryRouteListSuccess(PE::Segment *segment, const StreamSDK::RouteList &rl)
{
    StreamSDK::RESULT res = openStream(rl,StreamSDK::Stream::DIR_S2C,(StreamSDK::IContext*)STREAM_TYPE_MIXED);
}

void SegmentPlayerPrivate::onQueryRouteListFailed(PE::Segment *segment)
{

}

void SegmentPlayerPrivate::streamPlayEnd()
{
    if (m_pendStatus == SEG_PS_SEEKING) {
        return;
    }

    if (m_recvEndFlag == RECV_END_FLAG_NONE) {
        return;
    }
    bool bExpired = (m_recvEndFlag == RECV_END_FLAG_EXPIRED);

    doStop(true);

    m_status = (bExpired ? SEG_S_EXPIRED : SEG_S_END);

    emit reachEnd();
}

void SegmentPlayerPrivate::streamBufferAvailable(quint32 dwStreamType)
{
    Q_UNUSED(dwStreamType);
    bool bData = false;
    bool bFlag = false;
    StreamSDK::StreamData data;
    DWORD flags;
    {
        m_bPutable = true;

        if (m_dwRemainFlag != STREAM_FLAG_NONE) {
            bFlag = true;
            flags= m_dwRemainFlag;
        }

        if (m_remainData.IsValid()) {
            bData = true;
            data = m_remainData;
        }
    }

    BOOL bFailed = false;
    if (bFlag) {
        if (!inPutStreamFlag(flags)) {
            bFailed = true;
        }
    }

    if (!bFailed && bData) {
        if (!inPutStreamData(data)) {
            bFailed = true;
        }
    }

    if (!bFailed) {
        m_dwRemainFlag = STREAM_FLAG_NONE;
        m_remainData = StreamSDK::StreamData();
        m_stream.ResumeReceive();
    }
}

} // namespace Internal
} // namespace ClearVision
