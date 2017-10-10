#ifndef MEDIASOURCE_QUERY_WIDGET_H
#define MEDIASOURCE_QUERY_WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

namespace ProjectExplorer { class MediaSource; }

namespace MediaSource {
namespace Internal {

class MediaSource;
class QueryConditionWidget;
class TreeWidget;
class QueryWidget : public QWidget
{
    Q_OBJECT

public:
    enum QueryConditionRoles {
          QueryIdRole = Qt::UserRole
        , QueryPriorityRole
    };

public:
    explicit QueryWidget(QWidget *parent = 0);

public:
    void setCurrentMediaSource(ProjectExplorer::MediaSource *currentMediaSource);

private:
    void setupQueryConditionModels();
    void loadLastQueryConditions();

private slots:
    void queryTimeRangeChanged(int idx);
    void query();

private:
    QueryConditionWidget *m_queryConditionWidget;
    TreeWidget *m_queryResultWidget;

private:
    QStandardItemModel *m_queryConditionModel;
    ProjectExplorer::MediaSource *m_currentMediaSource;

private:
    static const quint8 m_frequentlyUsedRangeCount = 4;
};

} // namespace Internal
} // namespace MediaSource

#endif // MEDIASOURCE_QUERY_WIDGET_H
