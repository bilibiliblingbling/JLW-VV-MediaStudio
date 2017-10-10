#ifndef SEGMENT_H
#define SEGMENT_H

#include <QDateTime>

#include "metadata.h"
#include "projectexplorer_global.h"

namespace ProjectExplorer {

class Project;
class MediaSource;

class PROJECTEXPLORER_EXPORT Segment : public MetaData
{
    Q_OBJECT

public:
    enum RecordingType {
          ScheduleRecordingType
        , ManualRecordingType
        , AlarmRecordingType
    };

public:
    Segment(Core::Id id);
    Segment(const Segment &that);
    virtual ~Segment();

public:
    virtual Segment *clone() const = 0;

public:
    Core::Id mediaSourceId() const;
    Core::Id cameraId() const;
    qint32 recordingType() const;

    QDateTime bgnDateTime() const;
    QDateTime endDateTime() const;

    quint64 bgnTime() const;
    quint64 endTime() const;

public:
    Core::Id infoId() const;

public:
    QString mimeType() const Q_DECL_OVERRIDE;

protected:
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;

protected:
    void setMediaSourceId(Core::Id mediaSourceId);
    void setCameraId(Core::Id cameraId);
    void setRecordingType(qint32 recordingType);

    void setBeginTime(const QDateTime &dt);
    void setEndTime(const QDateTime &dt);

private:
    Core::Id m_mediaSourceId;
    Core::Id m_cameraId;
    qint32 m_recordingType;

    QDateTime m_bgnTime;    // GMT: Greenwich Mean Time
    QDateTime m_endTime;    // GMT: Greenwich Mean Time
};


class PROJECTEXPLORER_EXPORT Seg : public MetaData
{
    Q_OBJECT

public:
    Seg(Project *pro, Core::Id id);
    ~Seg();

    Project *project() const;

    QString name() const;
    void setName(const QString &name);

    quint32 ofstIn() const;
    quint32 ofstOut() const;

    void setOfstIn(quint32 in);
    void setOfstOut(quint32 out);

protected:
    bool fromMap(const QVariantMap &map) Q_DECL_OVERRIDE;
    QVariantMap toMap() const Q_DECL_OVERRIDE;

private:
    quint32 m_ofstIn;
    quint32 m_ofstOut;
    QString m_name;
};

} // namespace ProjectExplorer

#endif // SEGMENT_H
