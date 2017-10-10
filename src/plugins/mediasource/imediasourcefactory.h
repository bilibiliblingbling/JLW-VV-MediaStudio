#ifndef IMEDIASOURCE_FACTORY_H
#define IMEDIASOURCE_FACTORY_H

#include <QObject>

#include <core/id.h>

#include "cameratree.h"
#include "devicetree.h"
#include "mediasource_global.h"

namespace ProjectExplorer {
class BasicTree;
class MediaSource;
class ISelectTreeDialogFactory;
}

namespace MediaSource {

class MEDIASOURCE_EXPORT IMediaSourceFactory
{
public:
    IMediaSourceFactory(Core::Id classId, const QString &className);
    virtual ~IMediaSourceFactory();

public:
    Core::Id classId() const;
    QString className() const;

public:
    virtual QVariantList mediaSourceWizardRecipe() const;
    virtual ProjectExplorer::MediaSource *create(const QVariantMap &map) = 0;

public:
    virtual CameraTree *cameraTree() = 0;
    virtual DeviceTree *deviceTree() = 0;

    virtual bool queryRecordType() const;

private:
    Core::Id m_classId;
    QString m_className;
};

class MEDIASOURCE_EXPORT MediaSourceFactoryManager : public QObject
{
    Q_OBJECT

public:
    MediaSourceFactoryManager(QObject *parent = 0);
    ~MediaSourceFactoryManager();

    static MediaSourceFactoryManager *instance();

public:
    static QList<IMediaSourceFactory *> factories();
    static IMediaSourceFactory *factory(Core::Id id);

public:
    static void registerFactory(IMediaSourceFactory *factory);
};

} // namespace MediaSource

#endif // IMEDIASOURCE_FACTORY_H
