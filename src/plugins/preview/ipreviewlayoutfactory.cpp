#include <QDebug>
#include "constants.h"
#include "ipreviewlayoutfactory.h"

namespace Preview {
namespace Internal {

class PreviewLayoutFactory : public IPreviewLayoutFactory
{
public:
    PreviewLayoutFactory(Core::Id id, qint32 cols, qint32 rows = 2)
    {
        setId(id);
        setSize(rows, cols);
        setName(PreviewLayoutFactoryManager::tr("Layout %1x%2").arg(cols).arg(rows));
    }
};

static PreviewLayoutFactoryManager *s_previewlayoutfactorymanager = 0;

PreviewLayoutFactoryManager::PreviewLayoutFactoryManager(QObject *parent)
    : QObject(parent)
{
    setObjectName(QLatin1String("Preview.PreviewLayoutFactoryManager"));

    //set layout here
    {
        IPreviewLayoutFactory *factory =
                new PreviewLayoutFactory(Core::Id(Constants::PREVIEW_LAYOUT_4X2), 4);
        m_factories.insert(factory->id(), factory);
    }

    {
        IPreviewLayoutFactory *factory =
                new PreviewLayoutFactory(Core::Id(Constants::PREVIEW_LAYOUT_6X2), 6);
        m_factories.insert(factory->id(), factory);
    }

    {
        IPreviewLayoutFactory *factory =
                new PreviewLayoutFactory(Core::Id(Constants::PREVIEW_LAYOUT_8X2), 8);
        m_factories.insert(factory->id(), factory);
    }
    s_previewlayoutfactorymanager = this;
}

PreviewLayoutFactoryManager::~PreviewLayoutFactoryManager()
{
    s_previewlayoutfactorymanager = 0;

    qDeleteAll(m_factories);
}

PreviewLayoutFactoryManager *PreviewLayoutFactoryManager::instance()
{
    return s_previewlayoutfactorymanager;
}

QList<IPreviewLayoutFactory *> PreviewLayoutFactoryManager::factories()
{
    return s_previewlayoutfactorymanager->m_factories.values();
}

IPreviewLayoutFactory *PreviewLayoutFactoryManager::factory(Core::Id id)
{
    return s_previewlayoutfactorymanager->m_factories.value(id);
}

///////////////////////////////////////////////////////////////////////////////

IPreviewLayoutFactory::IPreviewLayoutFactory()
    : m_rows(0), m_columns(0)
{

}

IPreviewLayoutFactory::~IPreviewLayoutFactory()
{

}

QString IPreviewLayoutFactory::name() const
{
    return m_name;
}

Core::Id IPreviewLayoutFactory::id() const
{
    return m_id;
}

bool IPreviewLayoutFactory::isValid() const
{
    return (m_id.isValid() && m_rows && m_columns);
}

qint32 IPreviewLayoutFactory::rows() const
{
    return m_rows;
}

qint32 IPreviewLayoutFactory::columns() const
{
    return m_columns;
}

void IPreviewLayoutFactory::setName(const QString &name)
{
    m_name = name;
}

void IPreviewLayoutFactory::setId(Core::Id id)
{
    m_id = id;
}

void IPreviewLayoutFactory::setSize(qint32 rows, qint32 cols)
{
    m_rows = rows;
    m_columns = cols;
}

} // namespace Internal
} // namespace Preview




