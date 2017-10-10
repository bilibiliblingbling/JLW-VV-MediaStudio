#ifndef RESOURCE_H
#define RESOURCE_H

#include <QPixmap>
#include <QScopedPointer>

#include "constants.h"
#include "resource_global.h"

namespace Resource {

typedef enum {
    ContentBorder,
    HeaderBorder,
    TimeLineBarDown,
    TimeLineBarUp,
    TrackContent,
    TrackHeader,
} ScaledPixmapType;


typedef enum {
    // single
    MediaStudio,
    StretchLeftCur,
    StretchRightCur,

    // state: normal, hovered, pressed, disabled
    Close,
    Incise,

    Play,
    Pause,
    Stop,
    Prev,
    Next,

    Speed1_2,
    Speed1_4,
    Speed1x,
    Speed2x,
    Speed4x,

    TimeIn,
    TimeOut,
    T_TimeIn,
    T_TimeOut,
    T_ClearTimeInOut,

    V_Close,
    V_Play,
    V_Pause,
    V_Stop,
    V_Prev,
    V_Next,
    V_TimeIn,
    V_TimeOut,

    // (unchecked, checked) x (normal, hovered, pressed, disabled)
    Adsorb,
    LiveRpl,
    Output,
    Record,
    T_PlayTimeInOut,
    V_Record,

    // with menu,
    FrameShowMode,

    // normal
    GroupIcons,
    PaneIcons,
    RecordType,
    Slider,
    SmallIcons,
    Toolbar
} IndexPixmapType;



class MEDIASTUDIO_RESOURCE_EXPORT IndexPixmap : public QPixmap
{
public:
    IndexPixmap();
    IndexPixmap(const char *filename, quint32 cnt = 1);

public:
    void setWidth(quint32 w);
    QPixmap getSubPixmap(qint32 idx = -1) const;

private:
    class Impl;
    QSharedPointer<Impl> m_pimpl;
};


/// \todo
class ScaledPixmap : public QPixmap
{
public:
    ScaledPixmap();
    ScaledPixmap(const char *filename);
};


class MEDIASTUDIO_RESOURCE_EXPORT PixmapManager
{
    PixmapManager();

public:
    static PixmapManager &inst();

public:
    QPixmap getPixmap(ScaledPixmapType t);
    QPixmap getPixmap(ScaledPixmapType t, const QSize &s = QSize());
    QPixmap getPixmap(IndexPixmapType t, qint32 idx = -1);

private:
    class Impl;
    QScopedPointer<Impl> m_pimpl;
};

} // namespace Resource


#endif // RESOURCE_H
