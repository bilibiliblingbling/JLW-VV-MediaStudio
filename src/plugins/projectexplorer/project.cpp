#include <QDir>
#include <QFileInfo>

#include <utils/algorithm.h>
#include <utils/tmplog.h>

#include "basicnodes.h"
#include "constants.h"
#include "iprojectmanager.h"
#include "mediasource.h"
#include "monitor.h"
#include "project.h"
#include "segment.h"
#include "sequence.h"
#include "seqseg.h"

/*!
 *  工程文件中应存放什么?
 *      是否应该可扩展?
 *
 *  当没有 project 时应显示 default project(default session), 只包含相关
 *
 *
 */

namespace ProjectExplorer {
namespace Internal {
class ProjectPrivate
{
public:
    Core::Id m_id;
    Core::Context m_projectContext;

public:
#if 0
    QHash<Core::Id, QHash<Core::Id, MediaSource *> > m_mediaSources; // 媒体源列表, classId -> MediaSource
#else
    QHash<Core::Id, MediaSource *> m_mediaSrources;         // 媒体源列表
#endif
    QHash<Core::Id, Seg *> m_seqs;                          // 收藏夹中的媒体源片段
    QHash<Core::Id, Segment *> m_segments;                  // 所有媒体片段
    QHash<Core::Id, Monitor *> m_monitors;                  // 监视器列表
    QHash<Core::Id, FastPreview *> m_fastPreviews;          // 快速预览列表
    QHash<Core::Id, Sequence *> m_sequences;                // 输出序列列表, 数据存于 Node 中
};
} // namespace Internal


/*!
    使用 QVariantMap 存储工程数据,
      使用 map 存值, 使用方便, 扩展容易,


    原来的工程存储方式, 解析麻烦, 使用不便, 虽然扩展容易

 */

Project::Project()
    : d(new Internal::ProjectPrivate())
{
}

Project::~Project()
{
    qDeleteAll(d->m_mediaSrources);
    qDeleteAll(d->m_seqs);
    qDeleteAll(d->m_segments);
    qDeleteAll(d->m_monitors);
    qDeleteAll(d->m_fastPreviews);
    qDeleteAll(d->m_sequences);

    delete d;
}

Core::Id Project::id() const
{
    return d->m_id;
}

Core::Context Project::projectContext() const
{
    return d->m_projectContext;
}

bool Project::isValid() const
{
    return d->m_id.isValid();
}

/*!
 * \brief Project::addMediaSource
 * \param source
 *  添加媒体源到工程中
 */
void Project::addMediaSource(MediaSource *source)
{
    // TODO

    if (!source || !source->isValid()) {
        TM_DBG << "invalid MediaSource";
        return;
    }

    if (d->m_mediaSrources.contains(source->id())) {
        TM_DBG << "MediaSource of id:"
               << source->id().toString().toStdString()
               << "exists";
        return;
    }

    emit mediaSourceAboutToBeAdded(source);
    d->m_mediaSrources.insert(source->id(), source);
    emit mediaSourceAdded();

    // TODO: emit signal?
}

void Project::removeMediaSource(MediaSource *source)
{
    if (source && source->isValid()
        && d->m_mediaSrources.contains(source->id())) {
        emit mediaSourceAboutToBeRemoved(source);
        d->m_mediaSrources.remove(source->id());
        source->deleteLater();
        emit mediaSourceRemoved();
    }
}

QList<MediaSource *> Project::mediaSources() const
{
    return d->m_mediaSrources.values();
}

MediaSource *Project::mediaSource(Core::Id id)
{
    return d->m_mediaSrources.value(id);
}

/*!
 * \brief Project::addMonitor
 * \param monitor
 */
void Project::addMonitor(Monitor *monitor)
{
    // TODO
    if (!monitor || !monitor->isValid()) {
        TM_DBG << "invalid Monitor";
        return;
    }

    if (d->m_monitors.contains(monitor->id())) {
        TM_DBG << "Monitor of id:"
               << monitor->id().toString().toStdString()
               << "exists";
        return;
    }

    d->m_monitors.insert(monitor->id(), monitor);

    // TODO: emit signal?
}

bool Project::removeMonitor(Monitor *monitor)
{
    // TODO
    return true;
}

QList<Monitor *> Project::monitors() const
{
    return d->m_monitors.values();
}

void Project::addFastPreview(FastPreview *preview)
{
    // TODO
}

bool Project::removeFastPreview(FastPreview *preview)
{
    // TODO
    return true;
}

QList<FastPreview *> Project::fastPreviews() const
{
    return d->m_fastPreviews.values();
}

void Project::addSequence(Sequence *seq)
{
    // TODO
}

bool Project::removeSequence(Sequence *seq)
{
    // TODO

    return true;
}

QList<Sequence *> Project::sequences() const
{
    return d->m_sequences.values();
}

void Project::saveSettings()
{
    // TODO
}

bool Project::restoreSettings()
{
    //TODO

    return true;
}

void Project::setId(Core::Id id)
{
    d->m_id = id;
}

void Project::setProjectContext(Core::Context context)
{
    d->m_projectContext = context;
}

} // namespace ProjectExplorer
