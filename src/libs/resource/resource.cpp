#include "resource.h"

// single pixmap
#define RES_MEDIA_STUDIO                ":/res/mediastudio.ico"
#define RES_STRETCH_LEFT_CUR            ":/res/stretch_left_cur.ico"
#define RES_STRETCH_RIGHT_CUR           ":/res/stretch_right_cur.ico"


// scaled pixmap
#define RES_CONTENT_BORDER              ":/res/content_border.bmp"
#define RES_HEADER_BORDER               ":/res/header_border.bmp"
#define RES_TIME_LINE_BAR_DOWN          ":/res/timelinebar_down.bmp"
#define RES_TIME_LINE_BAR_UP            ":/res/timelinebar_up.bmp"
#define RES_TRACK_CONTENT               ":/res/track_content.bmp"
#define RES_TRACE_HEADER                ":/res/track_header.bmp"


// 1 state button pixmap
#define RES_CLOSE                       ":/res/close.bmp"
#define RES_INCISE                      ":/res/incise.bmp"

#define RES_PLAY                        ":/res/play.bmp"
#define RES_PAUSE                       ":/res/pause.bmp"
#define RES_STOP                        ":/res/stop.bmp"
#define RES_PREV                        ":/res/prev.bmp"
#define RES_NEXT                        ":/res/next.bmp"
#define RES_SPD_1_2                     ":/res/spd1-2.bmp"
#define RES_SPD_1_4                     ":/res/spd1-4.bmp"
#define RES_SPD_1X                      ":/res/spd1x.bmp"
#define RES_SPD_2X                      ":/res/spd2x.bmp"
#define RES_SPD_4X                      ":/res/spd4x.bmp"

#define RES_TIME_IN                     ":/res/timein.bmp"
#define RES_TIME_OUT                    ":/res/timeout.bmp"
#define RES_T_TIME_IN                   ":/res/t_timein.bmp"
#define RES_T_TIME_OUT                  ":/rse/t_timeout.bmp"
#define RES_T_CLEAR_TIME_IN_OUT         ":/res/t_clear_timeinout.bmp"

#define RES_V_CLOSE                     ":/res/v_close.bmp"
#define RES_V_PLAY                      ":/res/v_play.bmp"
#define RES_V_PAUSE                     ":/res/v_pause.bmp"
#define RES_V_STOP                      ":/res/v_stop.bmp"
#define RES_V_PREV                      ":/res/v_prev.bmp"
#define RES_V_NEXT                      ":/res/v_next.bmp"
#define RES_V_TIME_IN                   ":/res/v_timein.bmp"
#define RES_V_TIME_OUT                  ":/res/v_timeout.bmp"


// 2 states button pixmap
#define RES_ADSORB                      ":/res/adsorb.bmp"
#define RES_LIVE_RPL                    ":/res/liverpl.bmp"
#define RES_OUTPUT                      ":/res/outpu.bmp"
#define RES_RECORD                      ":/res/record.bmp"
#define RES_T_PLAY_TIME_IN_OUT          ":/res/t_play_timeinout.bmp"
#define RES_V_RECORD                    ":/res/v_record.bmp"


// 3 states button pixmap
#define RES_FRAME_SHOW_MODE             ":/res/frame_show_mode.bmp"


// normal
#define RES_GROUP_ICONS                 ":/res/group_icons.bmp"
#define RES_PANE_ICONS                  ":/res/paneicons.bmp"
#define RES_RECORD_TYPE_ICONS           ":/res/record_type.bmp"
#define RES_SLIDER                      ":/res/slider.bmp"
#define RES_SMALL_ICONS                 ":/res/smallicons.bmp"
#define RES_TOOLBAR                     ":/res/toolbar.bmp"



namespace Resource {


////////////////////////////////////////////////////////////////////////////////

class IndexPixmap::Impl
{
public:
    qint32 m_width;
    QHash<qint32, QPixmap> m_pixs;
};

IndexPixmap::IndexPixmap()
    : m_pimpl(new Impl())
{
    m_pimpl->m_width = 0;
}

IndexPixmap::IndexPixmap(const char *filename, quint32 cnt)
    : QPixmap(filename)
    , m_pimpl(new Impl())
{
    m_pimpl->m_width = width()/cnt;
}

void IndexPixmap::setWidth(quint32 w)
{
    m_pimpl->m_width = w;
}

QPixmap IndexPixmap::getSubPixmap(qint32 idx) const
{
    if (idx == -1)
        return *this;

    Q_ASSERT(idx*m_pimpl->m_width < width());

    if (!m_pimpl->m_pixs.contains(idx)) {
        m_pimpl->m_pixs.insert(idx, copy(idx*m_pimpl->m_width, 0, m_pimpl->m_width, height()));
    }

    return m_pimpl->m_pixs.value(idx);
}

////////////////////////////////////////////////////////////////////////////////


ScaledPixmap::ScaledPixmap()
{
}

ScaledPixmap::ScaledPixmap(const char *filename) :
    QPixmap(filename)
{
}

////////////////////////////////////////////////////////////////////////////////


class PixmapManager::Impl
{
public:
    QPixmap getPixmap(IndexPixmapType t, qint32 idx);
    QPixmap getPixmap(ScaledPixmapType, const QSize &s);

private:
    void loadPixmap(IndexPixmapType t);
    void loadPixmap(ScaledPixmapType t);

private:
    typedef QHash<IndexPixmapType, IndexPixmap> IndexPixmapSet;
    typedef QHash<ScaledPixmapType, ScaledPixmap> ScaledPixmapSet;

    IndexPixmapSet m_indexPixmaps;
    ScaledPixmapSet m_scaledPixmaps;
};


QPixmap PixmapManager::Impl::getPixmap(IndexPixmapType t, qint32 idx)
{
    if (!m_indexPixmaps.contains(t)) {
        loadPixmap(t);
    }

    return m_indexPixmaps.value(t).getSubPixmap(idx);
}

void PixmapManager::Impl::loadPixmap(IndexPixmapType t)
{
    IndexPixmap tmp;
    switch (t) {
    // single
    case MediaStudio:
        tmp = IndexPixmap(RES_MEDIA_STUDIO, 1);
        break;
    case StretchLeftCur:
        tmp = IndexPixmap(RES_STRETCH_LEFT_CUR, 1);
        break;
    case StretchRightCur:
        tmp = IndexPixmap(RES_MEDIA_STUDIO, 1);
        break;

    // state: normal, hovered, pressed, disabled
    case Close:
        tmp = IndexPixmap(RES_CLOSE, 4);
        break;
    case Incise:
        tmp = IndexPixmap(RES_INCISE, 4);
        break;

    case Play:
        tmp = IndexPixmap(RES_PLAY, 4);
        break;
    case Pause:
        tmp = IndexPixmap(RES_PAUSE, 4);
        break;
    case Stop:
        tmp = IndexPixmap(RES_STOP, 4);
        break;
    case Prev:
        tmp = IndexPixmap(RES_PREV, 4);
        break;
    case Next:
        tmp = IndexPixmap(RES_NEXT, 4);
        break;

    case Speed1_2:
        tmp = IndexPixmap(RES_SPD_1_2, 4);
        break;
    case Speed1_4:
        tmp = IndexPixmap(RES_SPD_1_4, 4);
        break;
    case Speed1x:
        tmp = IndexPixmap(RES_SPD_1X, 4);
        break;
    case Speed2x:
        tmp = IndexPixmap(RES_SPD_2X, 4);
        break;
    case Speed4x:
        tmp = IndexPixmap(RES_SPD_4X, 4);
        break;

    case TimeIn:
        tmp = IndexPixmap(RES_TIME_IN, 4);
        break;
    case TimeOut:
        tmp = IndexPixmap(RES_TIME_OUT, 4);
        break;
    case T_TimeIn:
        tmp = IndexPixmap(RES_T_TIME_IN, 4);
        break;
    case T_TimeOut:
        tmp = IndexPixmap(RES_T_TIME_OUT, 4);
        break;
    case T_ClearTimeInOut:
        tmp = IndexPixmap(RES_T_CLEAR_TIME_IN_OUT, 4);
        break;

    case V_Close:
        tmp = IndexPixmap(RES_V_CLOSE, 4);
        break;
    case V_Play:
        tmp = IndexPixmap(RES_V_PLAY, 4);
        break;
    case V_Pause:
        tmp = IndexPixmap(RES_V_PAUSE, 4);
        break;
    case V_Stop:
        tmp = IndexPixmap(RES_V_STOP, 4);
        break;
    case V_Prev:
        tmp = IndexPixmap(RES_V_PREV, 4);
        break;
    case V_Next:
        tmp = IndexPixmap(RES_V_NEXT, 4);
        break;
    case V_TimeIn:
        tmp = IndexPixmap(RES_V_TIME_IN, 4);
        break;
    case V_TimeOut:
        tmp = IndexPixmap(RES_V_TIME_OUT, 4);
        break;

    // (unchecked, checked) x (normal, hovered, pressed, disabled)
    case Adsorb:
        tmp = IndexPixmap(RES_ADSORB, 8);
        break;
    case LiveRpl:
        tmp = IndexPixmap(RES_LIVE_RPL, 8);
        break;
    case Output:
        tmp = IndexPixmap(RES_OUTPUT, 8);
        break;
    case Record:
        tmp = IndexPixmap(RES_RECORD, 8);
        break;
    case T_PlayTimeInOut:
        tmp = IndexPixmap(RES_T_PLAY_TIME_IN_OUT, 8);
        break;
    case V_Record:
        tmp = IndexPixmap(RES_V_RECORD, 8);
        break;

    // with menu,
    case FrameShowMode:
        tmp = IndexPixmap(RES_FRAME_SHOW_MODE, 12);
        break;

    // normal
    case GroupIcons:
        tmp = IndexPixmap(RES_GROUP_ICONS, 9);
        break;
    case PaneIcons:
        tmp = IndexPixmap(RES_PANE_ICONS, 2);
        break;
    case RecordType:
        tmp = IndexPixmap(RES_RECORD_TYPE_ICONS, 3);
        break;
    case Slider:
        tmp = IndexPixmap(RES_SLIDER, 2);
        break;
    case SmallIcons:
        tmp = IndexPixmap(RES_SMALL_ICONS);
        tmp.setWidth(16);
        break;
    case Toolbar:
        tmp = IndexPixmap(RES_MEDIA_STUDIO, 8);
        break;
    }

    Q_ASSERT(!tmp.isNull());
    m_indexPixmaps.insert(t, tmp);
}

void PixmapManager::Impl::loadPixmap(ScaledPixmapType t)
{
    ScaledPixmap tmp;
    switch (t) {
    case ContentBorder:
        tmp = ScaledPixmap(RES_CONTENT_BORDER);
        break;
    case HeaderBorder:
        tmp = ScaledPixmap(RES_HEADER_BORDER);
        break;
    case TimeLineBarDown:
        tmp = ScaledPixmap(RES_TIME_LINE_BAR_DOWN);
        break;
    case TimeLineBarUp:
        tmp = ScaledPixmap(RES_TIME_LINE_BAR_UP);
        break;
    case TrackContent:
        tmp = ScaledPixmap(RES_TRACK_CONTENT);
        break;
    case TrackHeader:
        tmp = ScaledPixmap(RES_TRACE_HEADER);
        break;
    }

    Q_ASSERT(!tmp.isNull());
    m_scaledPixmaps.insert(t, tmp);
}



////////////////////////////////////////////////////////////////////////////////

PixmapManager::PixmapManager() :
    m_pimpl(new Impl())
{

}

PixmapManager &PixmapManager::inst()
{
    static PixmapManager mgr;
    return mgr;
}

/*!
 * \brief PixmapManager::getPixmap
 * \param t
 * \param idx
 * \return a sub-indexed \c QPixmap of type \p t
 */
QPixmap PixmapManager::getPixmap(IndexPixmapType t, qint32 idx)
{
    return m_pimpl->getPixmap(t, idx);
}


} // namespace Resource
