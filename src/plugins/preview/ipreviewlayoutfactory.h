#ifndef IPREVIEW_LAYOUT_FACTORY_H
#define IPREVIEW_LAYOUT_FACTORY_H

#include <QHash>
#include <QObject>

#include <core/id.h>

namespace Preview {
namespace Internal {
class IPreviewLayoutFactory
{
public:
    IPreviewLayoutFactory();
    virtual ~IPreviewLayoutFactory();

public:
    QString name() const;
    Core::Id id() const;

    bool isValid() const;
    qint32 rows() const;
    qint32 columns() const;

protected:
    void setName(const QString &name);
    void setId(Core::Id id);
    void setSize(qint32 rows, qint32 cols);

private:
    QString m_name;
    Core::Id m_id;

    qint32 m_rows;
    qint32 m_columns;
};

class PreviewLayoutFactoryManager : public QObject
{
    Q_OBJECT
public:
    explicit PreviewLayoutFactoryManager(QObject *parent = 0);
    ~PreviewLayoutFactoryManager();

    static PreviewLayoutFactoryManager *instance();

public:
    static QList<IPreviewLayoutFactory *> factories();
    static IPreviewLayoutFactory *factory(Core::Id id);

private:
    QHash<Core::Id, IPreviewLayoutFactory *> m_factories;
};

} // namespace Internal
} // namespace Preview


#endif // IPREVIEW_LAYOUT_FACTORY_H
