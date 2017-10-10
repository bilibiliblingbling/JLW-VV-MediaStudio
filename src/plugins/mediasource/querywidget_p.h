#ifndef KINGVISION_QUERY_WIDGET_P_H
#define KINGVISION_QUERY_WIDGET_P_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include <core/id.h>
#include <projectexplorer/selecttreedialog.h>

#include "cameralistedit.h"

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class Camera;
class QueryConditionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueryConditionWidget(QWidget *parent = 0);

public:
    QComboBox *m_queryTimeRange;

    QCheckBox *m_queryTimeBeginEnabled;
    QCheckBox *m_queryTimeEndEnabled;
    QDateTimeEdit *m_queryTimeBegin;
    QDateTimeEdit *m_queryTimeEnd;

    QCheckBox *m_queryRecordType;
    QCheckBox *m_recordTypeSchedule;
    QCheckBox *m_recordTypeManual;
    QCheckBox *m_recordTypeAlarm;

    QCheckBox *m_queryCamerasEnabled;
    CameraListEdit *m_queryCameraList;

    QCheckBox *m_appendResults;
    QPushButton *m_queryButton;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_QUERY_WIDGET_P_H
