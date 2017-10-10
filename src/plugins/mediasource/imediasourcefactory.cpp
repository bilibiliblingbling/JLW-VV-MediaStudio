#include <QHash>
#include <QVariantList>

#include "imediasourcefactory.h"

namespace MediaSource {

/*!
 * all \c IMediaSourceFactory are managed by \c MediaSourceFactoryManager.
 * all \c IMediaSourceFactory should be create in heap, and it's the
 * \c MediaSourceFactoryManager's responsibility to free them.
 */

IMediaSourceFactory::IMediaSourceFactory(Core::Id classId, const QString &className)
    : m_classId(classId), m_className(className)
{
    MediaSourceFactoryManager::registerFactory(this);
}

IMediaSourceFactory::~IMediaSourceFactory()
{
}

Core::Id IMediaSourceFactory::classId() const
{
    return m_classId;
}

QString IMediaSourceFactory::className() const
{
    return m_className;
}

QVariantList IMediaSourceFactory::mediaSourceWizardRecipe() const
{
    // TODO: will be pure virtual in the future
    QVariantList recipe;
    return recipe;
}

bool IMediaSourceFactory::queryRecordType() const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////

namespace Internal {
class MediaSourceFactoryManagerPrivate
{
public:
    QHash<Core::Id, IMediaSourceFactory *> m_factories;
};
} // namespace Internal

static MediaSourceFactoryManager *s_mediasourcefactorymanager = 0;
static Internal::MediaSourceFactoryManagerPrivate *s_mediasourcefactorymanagerprivate = 0;
MediaSourceFactoryManager::MediaSourceFactoryManager(QObject *parent)
    : QObject(parent)
{
    setObjectName(QLatin1String("MediaSource.MediaSourceFactoryManager"));

    s_mediasourcefactorymanagerprivate = new Internal::MediaSourceFactoryManagerPrivate();
    s_mediasourcefactorymanager = this;
}

MediaSourceFactoryManager::~MediaSourceFactoryManager()
{
    s_mediasourcefactorymanager = 0;

    // free all factories
    qDeleteAll(s_mediasourcefactorymanagerprivate->m_factories);

    delete s_mediasourcefactorymanagerprivate;
    s_mediasourcefactorymanagerprivate = 0;
}

MediaSourceFactoryManager *MediaSourceFactoryManager::instance()
{
    return s_mediasourcefactorymanager;
}

QList<IMediaSourceFactory *> MediaSourceFactoryManager::factories()
{
    return s_mediasourcefactorymanagerprivate->m_factories.values();
}

IMediaSourceFactory *MediaSourceFactoryManager::factory(Core::Id id)
{
    return s_mediasourcefactorymanagerprivate->m_factories.value(id);
}

void MediaSourceFactoryManager::registerFactory(IMediaSourceFactory *factory)
{
    s_mediasourcefactorymanagerprivate->m_factories.insert(factory->classId(), factory);
}

} // namespace MediaSource
