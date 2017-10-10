#include <QDir>
#include <QDomDocument>
#include <QFileInfo>
#include <QSettings>

#include <mediasource/constants.h>
#include <mediasource/imediasourcefactory.h>
#include <projectexplorer/constants.h>
#include <projectexplorer/mediasource.h>
#include <utils/tmplog.h>

#include "constants.h"
#include "genericproject.h"

namespace GenericProjectManager {
namespace Internal {

namespace {

#define FALSE_RETURN(expr) \
    do { if (!expr) return false; } while (0)

#define LITERAL_EXPR(expr) \
    do { expr; } while (0)

#if 0
#define PARSE_ERROR_RETURN(expr)    FALSE_RETURN(expr)
#else
#define PARSE_ERROR_RETURN(expr)    LITERAL_EXPR(expr)
#endif


void appendErrorString(QString *errorString, const QString &errorMsg)
{
    if (errorString) {
        if (!errorString->isEmpty())
            *errorString += QStringLiteral("\n");

        *errorString += errorMsg;
    }
}

bool loadXml(QDomDocument &doc, const QString &filename, QString *errorString)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        appendErrorString(errorString,
                          GenericProject::tr("Failed to open file `%1': %2").arg(filename, file.errorString()));
        return false;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    if (!doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        appendErrorString(errorString,
                          GenericProject::tr("Failed to parse file `%1' at line %2, column %3: %4")
                          .arg(filename, QString::number(errorLine), QString::number(errorColumn), errorMsg));
        return false;
    }

    file.close();

    return true;
}



/*! @todo '2.25' 重写递归让其实现相同Key的Value 放到list中保存在一个KEY中,
 *  但是希望被其他人优化一下,
 *  现存问题:多个子元素,无明显标识,用MULTIMAP也存在这个问题*/

void traversal(const QDomElement &parent, QVariantMap &map)
{
    QDomElement child = parent.firstChildElement();
    for (; !child.isNull(); child = child.nextSiblingElement()) {
        const QString tag = child.tagName();
        Q_ASSERT(!tag.isEmpty());   // continue
        if (child.firstChildElement().isNull()) {
            map.insert(tag, child.text());
        } else if (child.tagName() == child.nextSiblingElement().tagName()){
            QVariantList sublist;
            for(; !child.isNull(); child = child.nextSiblingElement()) {
                QVariantMap submap;
                traversal(child, submap);
                sublist.append(submap);
            }
            map.insert(tag, sublist);
        } else {
            QVariantMap submap;
            traversal(child, submap);
            map.insert(tag, submap);
        }
    }
}

} // namespace anonymous

namespace PE    = ProjectExplorer;
namespace PEC   = ProjectExplorer::Constants;
namespace MS    = MediaSource;
namespace MSC   = MediaSource::Constants;

GenericProject::GenericProject(GenericProjectManager *manager, const QString &filename)
    : m_manager(manager)
    , m_projectFileName(filename)
{
}

GenericProject::~GenericProject()
{
}

bool GenericProject::parseSettings(QString *errorString)
{
    if (!parseProjectInfo(errorString))
        return false;

    PARSE_ERROR_RETURN(parseMediaSourceInfo(errorString));
    PARSE_ERROR_RETURN(parseMediaDestinationInfo(errorString));
    PARSE_ERROR_RETURN(parseMediaFileInfo(errorString));
    PARSE_ERROR_RETURN(parseFastPreviewInfo(errorString));
    PARSE_ERROR_RETURN(parsePlayMonitorInfo(errorString));
    PARSE_ERROR_RETURN(parseSequenceInfo(errorString));

    return true;
}

QString GenericProject::projectFilePath() const
{
    return m_projectFileName;
}

QString GenericProject::displayName() const
{
    return m_projectName;
}

bool GenericProject::parseProjectInfo(QString *errorString)
{
    QFileInfo fileInfo(m_projectFileName);

    QSettings mstproj(m_projectFileName, QSettings::IniFormat);

    Core::Id proid = Core::Id::fromSetting(mstproj.value(QStringLiteral("ID")));
    if (!proid.isValid()) {
        appendErrorString(errorString,
                          tr("Invalid project `%1': no valid project id").arg(fileInfo.absoluteFilePath()));
        return false;
    }

    setId(proid);
    m_projectName = mstproj.value(QStringLiteral("Name")).toString();

    QString filePath = fileInfo.absolutePath();

    m_mediaSourceFileName
            = filePath + QDir::toNativeSeparators(QStringLiteral("/"))
              + mstproj.value(QStringLiteral("MediaSrcArchive")).toString();

    m_mediaDestinationFileName
            = filePath + QDir::toNativeSeparators(QStringLiteral("/"))
              + mstproj.value(QStringLiteral("MediaDstArchive")).toString();

    m_mediaFileFileName
            = filePath + QDir::toNativeSeparators(QStringLiteral("/"))
              + mstproj.value(QStringLiteral("MediaFileArchive")).toString();

    m_fastPreviewFileName
            = filePath + QDir::toNativeSeparators("/")
              + mstproj.value(QStringLiteral("FastPreviewArchive")).toString();

    m_playMonitorFileName
            = filePath + QDir::toNativeSeparators(QStringLiteral("/"))
              + mstproj.value(QStringLiteral("MonitorArchive")).toString();

    m_sequenceFileName
            = filePath + QDir::toNativeSeparators(QStringLiteral("/"))
              + mstproj.value(QStringLiteral("SequenceArchive")).toString();


    return true;
}

bool GenericProject::parseMediaSourceInfo(QString *errorString)
{
    QDomDocument doc;
    if (!loadXml(doc, m_mediaSourceFileName, errorString))
        return false;

    QDomElement mediasources = doc.firstChildElement(QStringLiteral("MediaSources"));
    if (mediasources.isNull()) {
        appendErrorString(errorString,
                          tr("Invalid MediaSource file `%1': node `%2' not found")
                          .arg(m_mediaSourceFileName, QStringLiteral("MediaSources")));
        return false;
    }

    for (QDomElement mediasourceclass = mediasources.firstChildElement(QStringLiteral("MediaSourceClass"));
         !mediasourceclass.isNull();
         mediasourceclass = mediasourceclass.nextSiblingElement(QStringLiteral("MediaSourceClass"))) {

        Core::Id clsId = Core::Id::fromSetting(mediasourceclass.attribute(QStringLiteral("id")));
        Q_ASSERT_X(clsId.isValid(), "media source", "class id can not be empty");

        auto mediasourcefactory = MS::MediaSourceFactoryManager::factory(clsId);

        if (!mediasourcefactory) {
            TM_DBG("MediaSource of class `%s' is not support", clsId.toString().toStdString().c_str());
            appendErrorString(errorString,
                              tr("MediaSource of class `%1' is not support").arg(clsId.toString()));
            continue;
        }

        for (QDomElement mediasource = mediasourceclass.firstChildElement(QStringLiteral("MediaSource"));
             !mediasource.isNull();
             mediasource = mediasource.nextSiblingElement(QStringLiteral("MediaSource"))) {

            Core::Id id = Core::Id::fromSetting(mediasource.attribute(QStringLiteral("id")));
            if (!id.isValid()) {
                TM_DBG << "Invalid MediaSource: no valid id";
                continue;
            }

            QVariantMap sourcemap;
            sourcemap.insert(PEC::MEDIASOURCE_CLASSID, clsId.toSetting());
            sourcemap.insert(PEC::METADATA_ID, id.toSetting());

            traversal(mediasource, sourcemap);

            auto source = mediasourcefactory->create(sourcemap);
            if (source) {
                addMediaSource(source);
            }
        }
    }

    return true;
}

bool GenericProject::parseMediaDestinationInfo(QString *errorString)
{
    QDomDocument doc;
    if (!loadXml(doc, m_mediaDestinationFileName, errorString))
        return false;

    // destination class
    // TODO: 需要有单独的 destination/output 插件, 解码器部分

    return true;
}

bool GenericProject::parseMediaFileInfo(QString *errorString)
{
    QDomDocument doc;
    if (!loadXml(doc, m_mediaFileFileName, errorString))
        return false;

    QDomElement mediafiles = doc.firstChildElement(QStringLiteral("MediaFiles"));
    if (mediafiles.isNull()) {
        appendErrorString(errorString,
                          tr("Invalid MediaFile file `%1': node `%2' not found")
                          .arg(m_mediaFileFileName, QStringLiteral("MediaFiles")));
        return false;
    }

    do {
        // MediaSource related files
        QDomElement mediasourcefiles = mediafiles.firstChildElement(QStringLiteral("MediaSourceFiles"));
        if (mediasourcefiles.isNull()) {
            appendErrorString(errorString,
                              tr("Invalid MediaFile file `%1': node `%2' not found")
                              .arg(m_mediaFileFileName, QStringLiteral("MediaSourceFiles")));
            break;
        }

        for (QDomElement mediasourcefile = mediasourcefiles.firstChildElement(QStringLiteral("File"));
             !mediasourcefile.isNull();
             mediasourcefile = mediasourcefile.nextSiblingElement(QStringLiteral("File"))) {

            QDomElement fileid = mediasourcefile.firstChildElement(QStringLiteral("ID"));
            if (fileid.isNull())
                continue;

            Core::Id fid = Core::Id::fromString(fileid.text());
            if (!fid.isValid())
                continue;

            QDomElement sourceid = mediasourcefile.firstChildElement(QStringLiteral("SourceID"));
            if (sourceid.isNull())
                continue;

            Core::Id sid = Core::Id::fromString(sourceid.text());
            if (!sid.isValid())
                continue;

            PE::MediaSource *ms = mediaSource(sid);
            if (!ms)
                continue;

            QVariantMap mediasourcefilemap;
            mediasourcefilemap.insert(QLatin1String(PEC::METADATA_ID), fid.toSetting());
            mediasourcefilemap.insert(QLatin1String(PEC::SEGMENT_MEDIASOURCEID), sid.toSetting());

            QDomElement aspect = mediasourcefile.firstChildElement(QStringLiteral("Aspects"));
            QVariantMap aspectmap;
            traversal(aspect, aspectmap);

            // TODO: 可能需要修改 segment 定义, 考虑沿用 file 的定义
            mediasourcefilemap.insert(QLatin1String("Aspects"), aspectmap);

            // ms->createSegment();
        }
    } while (0);

    do {
        // Destination related files
        QDomElement mediadestinationfiles = mediafiles.firstChildElement(QStringLiteral("MediaDestinationFiles"));
        if (mediadestinationfiles.isNull()) {
            appendErrorString(errorString,
                              tr("Invalid MediaFile file `%1': node `%2' not found")
                              .arg(m_mediaFileFileName, QStringLiteral("MediaDestinationFiles")));
            break;
        }

        for (QDomElement mediadestinationfile = mediadestinationfiles.firstChildElement(QStringLiteral("File"));
             !mediadestinationfile.isNull();
             mediadestinationfile = mediadestinationfile.nextSiblingElement(QStringLiteral("File"))) {
            // TODO: 输出用, 需要引用输出设备
        }
    } while (0);

    do {
        // MediaSource file repository
        QDomElement mediasourcefilelibrary = mediafiles.firstChildElement(QStringLiteral("MediaSourceFileLibrary"));
        if (mediasourcefilelibrary.isNull()) {
            appendErrorString(errorString,
                              tr("Invalid MediaFile file `%1': node `%2' not found")
                              .arg(m_mediaFileFileName, QStringLiteral("MediaSourceFileLibrary")));
            break;
        }

        for (QDomElement mediasourcefileitem = mediasourcefilelibrary.firstChildElement(QStringLiteral("Item"));
             !mediasourcefileitem.isNull();
             mediasourcefileitem = mediasourcefileitem.nextSiblingElement(QStringLiteral("Item"))) {
            // TODO:
        }
    } while (0);

    return true;
}

bool GenericProject::parseFastPreviewInfo(QString *errorString)
{
    QDomDocument doc;
    if (!loadXml(doc, m_fastPreviewFileName, errorString))
        return false;

    QDomElement fastpreviews = doc.firstChildElement(QStringLiteral("FastPreviews"));
    if (fastpreviews.isNull()) {
        appendErrorString(errorString,
                          tr("Invalid FastPreviews file `%1': node `%2' not found")
                          .arg(m_fastPreviewFileName, QStringLiteral("FastPreviews")));
        return false;
    }

    quint32 count = fastpreviews.attribute(QStringLiteral("count")).toUInt();
    // TODO: set layout

    for (QDomElement fastpreview = fastpreviews.firstChildElement(QStringLiteral("FastPreview"));
         !fastpreview.isNull();
         fastpreview = fastpreview.nextSiblingElement(QStringLiteral("FastPreview"))) {
        // TODO:
        // traversal(fastpreview, QVariantMap());
        // PE::FastPreview *fp = 0;
        // addFastPreview(fp);
    }

    return true;
}

bool GenericProject::parsePlayMonitorInfo(QString *errorString)
{
    QDomDocument doc;
    if (!loadXml(doc, m_playMonitorFileName, errorString))
        return false;

    QDomElement playmonitors = doc.firstChildElement(QStringLiteral("PlayMonitors"));
    if (playmonitors.isNull()) {
        appendErrorString(errorString,
                          tr("Invalid PlayMonitors file `%1': node `%2' not found")
                          .arg(m_playMonitorFileName, QStringLiteral("PlayMonitors")));
        return false;
    }

    do {
        // file play monitor
        QDomElement fileplaymonitor = playmonitors.firstChildElement(QStringLiteral("FilePlayMonitor"));
        if (fileplaymonitor.isNull()) {
            appendErrorString(errorString,
                              tr("Invalid PlayMonitors file `%1': node `%2' not found")
                              .arg(m_playMonitorFileName, QStringLiteral("FilePlayMonitor")));
            break;
        }
        // TODO: 需要先完善 monitor
    } while (0);

    do {
        // sequence play monitor
        QDomElement sequenceplaymonitor = playmonitors.firstChildElement(QStringLiteral("SequencePlayMonitor"));
        if (sequenceplaymonitor.isNull()) {
            appendErrorString(errorString,
                              tr("Invalid PlayMonitors file `%1': node `%2' not found")
                              .arg(m_playMonitorFileName, QStringLiteral("SequencePlayMonitor")));
            break;
        }
        // TODO: 需要先完善 sequence monitor
    } while (0);

    return true;
}

bool GenericProject::parseSequenceInfo(QString *errorString)
{
    QDomDocument doc;
    if (!loadXml(doc, m_sequenceFileName, errorString))
        return false;

    QDomElement sequences = doc.firstChildElement(QStringLiteral("Sequences"));
    if (sequences.isNull()) {
        appendErrorString(errorString,
                          tr("Invalid Sequences file `%1': node `%2' not found")
                          .arg(m_sequenceFileName, QStringLiteral("Sequences")));
        return false;
    }

    // TODO: 先完善时间线部分

    return true;
}

} // namespace Internal
} // namespace GenericProjectManager
