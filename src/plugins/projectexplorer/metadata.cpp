#include "constants.h"
#include "metadata.h"

namespace ProjectExplorer {


MetaData::~MetaData()
{
}

Core::Id MetaData::id() const
{
    return m_id;
}

bool MetaData::isValid() const
{
    return m_id.isValid();
}

QString MetaData::mimeType() const
{
    return  QLatin1String(Constants::METADATE_MIMETYPE);
}

bool MetaData::fromMap(const QVariantMap &map)
{
    m_id = Core::Id::fromSetting(map.value(QLatin1String(Constants::METADATA_ID)));
    return m_id.isValid();
}

QVariantMap MetaData::toMap() const
{
    QVariantMap map;
    map.insert(QLatin1String(Constants::METADATA_ID), m_id.toSetting());
    return map;
}

MetaData::MetaData(QObject *parent)
    : QObject(parent)
{
}

MetaData::MetaData(QObject *parent, Core::Id id)
    : QObject(parent)
    , m_id(id)
{
}

MetaData::MetaData(const MetaData &that)
    : QObject(that.parent())
    , m_id(that.m_id)
{
}

void MetaData::setId(Core::Id id)
{
    m_id = id;
}



} // namespace ProjectExplorer
