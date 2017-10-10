#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardItemModel>
#include <QToolButton>

#include <projectexplorer/mediasource.h>
#include <utils/tmplog.h>

#include "constants.h"
#include "imediasourcefactory.h"
#include "iquerytimerangefactory.h"
#include "querywidget.h"
#include "querywidget_p.h"
#include "segmenttree.h"
#include "treewidget.h"

namespace MediaSource {
namespace Internal {

QueryWidget::QueryWidget(QWidget *parent)
    : QWidget(parent)
    , m_queryConditionWidget(0)
    , m_queryResultWidget(0)
    , m_queryConditionModel(0)
    , m_currentMediaSource(0)
{
    m_queryConditionWidget = new QueryConditionWidget(this);
    m_queryResultWidget = new TreeWidget(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_queryConditionWidget);
    layout->addWidget(m_queryResultWidget, 1);
    setLayout(layout);

    m_queryConditionModel = new QStandardItemModel(this);
    m_queryConditionModel->setSortRole(QueryPriorityRole);
    setupQueryConditionModels();

    m_queryConditionWidget->m_queryTimeRange->setModel(m_queryConditionModel);

    connect(m_queryConditionWidget->m_queryTimeRange, SIGNAL(currentIndexChanged(int)),
            this, SLOT(queryTimeRangeChanged(int)));
    connect(m_queryConditionWidget->m_queryButton, SIGNAL(clicked()), this, SLOT(query()));

    connect(m_queryConditionWidget->m_queryTimeBegin, &QDateTimeEdit::dateTimeChanged,
            this, [this](const QDateTime &dt){
        m_queryConditionWidget->m_queryTimeEnd->setMinimumDateTime(dt);
    });
    connect(m_queryConditionWidget->m_queryTimeEnd, &QDateTimeEdit::dateTimeChanged,
            this, [this](const QDateTime &dt){
        m_queryConditionWidget->m_queryTimeBegin->setMaximumDateTime(dt);
    });

    m_queryConditionWidget->m_queryTimeBeginEnabled->setChecked(true);
    m_queryConditionWidget->m_queryTimeEndEnabled->setChecked(true);
    m_queryConditionWidget->m_queryCamerasEnabled->setChecked(true);
    queryTimeRangeChanged(m_queryConditionWidget->m_queryTimeRange->currentIndex());

    // TODO: show camera select tree
}

void QueryWidget::setCurrentMediaSource(ProjectExplorer::MediaSource *currentMediaSource)
{
    if (m_currentMediaSource == currentMediaSource)
        return;

    m_currentMediaSource = currentMediaSource;

    IMediaSourceFactory *factory
            = MediaSourceFactoryManager::factory(currentMediaSource->classId());

    // setup query condition widet
    bool queryRecordType = factory->queryRecordType();
    m_queryConditionWidget->m_queryRecordType->setVisible(queryRecordType);
    bool v = (queryRecordType && m_queryConditionWidget->m_queryRecordType->isChecked());
    m_queryConditionWidget->m_recordTypeSchedule->setVisible(v);
    m_queryConditionWidget->m_recordTypeManual->setVisible(v);
    m_queryConditionWidget->m_recordTypeAlarm->setVisible(v);

    auto cameraTree = factory->cameraTree();
    cameraTree->setCurrentMediaSource(m_currentMediaSource);
    m_queryConditionWidget->m_queryCameraList->setCameraTree(cameraTree);

    SegmentTree::setCurrentMediaSource(m_currentMediaSource);
    m_queryResultWidget->setBasicTree(SegmentTree::instance());

    if (DeviceTree *dt = factory->deviceTree()) {
        dt->disconnect(m_queryConditionWidget->m_queryCameraList);
        connect(dt, &DeviceTree::aboutToQueryCameras,
                m_queryConditionWidget->m_queryCameraList, &CameraListEdit::setCameras);
    }

    loadLastQueryConditions();
}

void QueryWidget::setupQueryConditionModels()
{
    auto querytimerangefactories = QueryTimeRangeFactoryManager::factories();

    foreach (auto factory, querytimerangefactories) {
        QStandardItem *item = new QStandardItem(factory->displayName());
        item->setData(factory->id().toSetting(), QueryIdRole);
        item->setData(factory->priority(), QueryPriorityRole);
        m_queryConditionModel->appendRow(item);
    }

    m_queryConditionModel->sort(0, Qt::AscendingOrder);
}

void QueryWidget::loadLastQueryConditions()
{
    m_queryConditionWidget->m_queryCamerasEnabled->setChecked(true);
    m_queryConditionWidget->m_queryCameraList->setEnabled(true);

    QVariantMap conditions = m_currentMediaSource->lastQueryConditions();
    if (!conditions.empty()) {
        // time range
        QVariant id = conditions.value(QLatin1String(Constants::MEDIASOURCE_QUERY_TIMERANGEID));
        IQueryTimeRangeFactory *factory
                = QueryTimeRangeFactoryManager::factory(Core::Id::fromSetting(id));
        if (factory) {
            m_queryConditionWidget->m_queryTimeRange->setCurrentText(factory->displayName());
            if (!factory->dynamicTimeRange()) {
                m_queryConditionWidget->m_queryTimeBeginEnabled->setChecked(true);
                m_queryConditionWidget->m_queryTimeBegin->setDateTime(factory->queryTimeBegin());
                m_queryConditionWidget->m_queryTimeEndEnabled->setChecked(true);
                m_queryConditionWidget->m_queryTimeEnd->setDateTime(factory->queryTimeEnd());
            }
        }

        // record type
        int recordType =
                conditions.value(QLatin1String(Constants::MEDIASOURCE_QUERY_RECORDTYPE), Constants::MEDIASOURCE_RECORDTYPE_ALL).toInt();
        m_queryConditionWidget->m_recordTypeSchedule->setChecked(recordType & Constants::MEDIASOURCE_RECORDTYPE_SCHEDULE);
        m_queryConditionWidget->m_recordTypeAlarm->setChecked(recordType & Constants::MEDIASOURCE_RECORDTYPE_ALARM);
        m_queryConditionWidget->m_recordTypeManual->setChecked(recordType & Constants::MEDIASOURCE_RECORDTYPE_MANUAL);

        // camera list
        QList<Core::Id> cameras;
        {
            QStringList camlst = conditions.value(QLatin1String(Constants::MEDIASOURCE_QUERY_CAMERALIST)).toStringList();
            foreach (auto cam, camlst) {
                Core::Id id = Core::Id::fromString(cam);
                if (id.isValid())
                    cameras << id;
            }
        }
        m_queryConditionWidget->m_queryCameraList->setCameras(cameras);

        // append
        bool append = conditions.value(QLatin1String(Constants::MEDIASOURCE_QUERY_APPENDRESULTS), false).toBool();
        m_queryConditionWidget->m_appendResults->setChecked(append);
    } else {
        // no last query condition
        m_queryConditionWidget->m_queryCameraList->setCameras(QList<Core::Id>());
    }
}

void QueryWidget::queryTimeRangeChanged(int idx)
{
    if (idx == -1)
        return;

    Q_ASSERT(m_queryConditionModel->rowCount() > idx);

    QStandardItem *item = m_queryConditionModel->item(idx);
    Q_ASSERT(item);

    QVariant id = item->data(QueryIdRole);
    IQueryTimeRangeFactory *factory
            = QueryTimeRangeFactoryManager::factory(Core::Id::fromSetting(id));

    if (factory) {
        bool hide = factory->dynamicTimeRange();
        m_queryConditionWidget->m_queryTimeBeginEnabled->setHidden(hide);
        m_queryConditionWidget->m_queryTimeEndEnabled->setHidden(hide);
        m_queryConditionWidget->m_queryTimeBegin->setHidden(hide);
        m_queryConditionWidget->m_queryTimeEnd->setHidden(hide);

        m_queryConditionWidget->m_queryTimeBegin->setEnabled(
                    m_queryConditionWidget->m_queryTimeBeginEnabled->isChecked());
        m_queryConditionWidget->m_queryTimeEnd->setEnabled(
                    m_queryConditionWidget->m_queryTimeEndEnabled->isChecked());

        if (!hide) {
            m_queryConditionWidget->m_queryTimeBegin->setDateTime(factory->queryTimeBegin());
            m_queryConditionWidget->m_queryTimeEnd->setDateTime(factory->queryTimeEnd());
        }
    }
}

void QueryWidget::query()
{
    QVariantMap conditions;
    QStringList camlst;
    if (m_queryConditionWidget->m_queryCamerasEnabled->isChecked()) {
        foreach (auto camera, m_queryConditionWidget->m_queryCameraList->cameras()) {
            camlst << camera.toString();
        }
    }
    conditions.insert(QLatin1String(Constants::MEDIASOURCE_QUERY_CAMERALIST), camlst);

    if (m_queryConditionWidget->m_queryRecordType->isVisible()) {
        int recordType = 0;
        if (m_queryConditionWidget->m_queryRecordType->isChecked()) {
            if (m_queryConditionWidget->m_recordTypeSchedule->isChecked())
                recordType |= Constants::MEDIASOURCE_RECORDTYPE_SCHEDULE;

            if (m_queryConditionWidget->m_recordTypeManual->isChecked())
                recordType |= Constants::MEDIASOURCE_RECORDTYPE_MANUAL;

            if (m_queryConditionWidget->m_recordTypeAlarm->isChecked())
                recordType |= Constants::MEDIASOURCE_RECORDTYPE_ALARM;
        } else {
            recordType |= Constants::MEDIASOURCE_RECORDTYPE_ALL;
        }

        conditions.insert(QLatin1String(Constants::MEDIASOURCE_QUERY_RECORDTYPE), recordType);
    }

    quint64 bgnTime = 0;
    quint64 endTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
    int idx = m_queryConditionWidget->m_queryTimeRange->currentIndex();
    if (idx != -1) {
        QStandardItem *item = m_queryConditionModel->item(idx);
        conditions.insert(QLatin1String(Constants::MEDIASOURCE_QUERY_TIMERANGEID), item->data(QueryIdRole));

        IQueryTimeRangeFactory *factory
                = QueryTimeRangeFactoryManager::factory(Core::Id::fromSetting(item->data(QueryIdRole)));
        if (factory->dynamicTimeRange()) {
            bgnTime = factory->queryTimeBegin().toMSecsSinceEpoch()/1000;
            endTime = factory->queryTimeEnd().toMSecsSinceEpoch()/1000;
        } else {
            QDateTime tmpBgn, tmpEnd;
            if (m_queryConditionWidget->m_queryTimeBeginEnabled->isChecked()) {
                tmpBgn = m_queryConditionWidget->m_queryTimeBegin->dateTime();
                bgnTime = tmpBgn.toMSecsSinceEpoch()/1000;
            }
            if (m_queryConditionWidget->m_queryTimeEndEnabled->isChecked()) {
                tmpEnd = m_queryConditionWidget->m_queryTimeEnd->dateTime();
                endTime = tmpEnd.toMSecsSinceEpoch()/1000;
            }
            factory->updateTimeRange(tmpBgn, tmpEnd);
        }
    }

    conditions.insert(QLatin1String(Constants::MEDIASOURCE_QUERY_BGNTIME), bgnTime);
    conditions.insert(QLatin1String(Constants::MEDIASOURCE_QUERY_ENDTIME), endTime);
    conditions.insert(QLatin1String(Constants::MEDIASOURCE_QUERY_APPENDRESULTS),
                      m_queryConditionWidget->m_appendResults->isChecked());

    m_currentMediaSource->queryRecordings(conditions);
}


////////////////////////////////////////////////////////////////////////////////


QueryConditionWidget::QueryConditionWidget(QWidget *parent)
    : QWidget(parent)
{
    // time range
    QLabel *timeRange = new QLabel(tr("Time &Range"), this);
    m_queryTimeRange = new QComboBox(this);
    m_queryTimeRange->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    timeRange->setBuddy(m_queryTimeRange);

    // time
    m_queryTimeBeginEnabled = new QCheckBox(tr("Begin Time"), this);
    m_queryTimeBegin = new QDateTimeEdit(this);
    m_queryTimeEndEnabled = new QCheckBox(tr("End Time"), this);
    m_queryTimeEnd = new QDateTimeEdit(this);

    m_queryTimeBegin->setCalendarPopup(true);
    m_queryTimeEnd->setCalendarPopup(true);
    m_queryTimeBegin->setDisplayFormat(QLatin1String("yyyy-MM-dd hh:mm:ss"));
    m_queryTimeEnd->setDisplayFormat(QLatin1String("yyyy-MM-dd hh:mm:ss"));
    connect(m_queryTimeBeginEnabled, SIGNAL(clicked(bool)),
            m_queryTimeBegin, SLOT(setEnabled(bool)));
    connect(m_queryTimeEndEnabled, SIGNAL(clicked(bool)),
            m_queryTimeEnd, SLOT(setEnabled(bool)));

    // record types
    m_queryRecordType = new QCheckBox(tr("Filter Recording Types"), this);
    m_recordTypeSchedule = new QCheckBox(tr("Schedule"), this);
    m_recordTypeManual = new QCheckBox(tr("Manual"), this);
    m_recordTypeAlarm = new QCheckBox(tr("Alarm"), this);
    connect(m_queryRecordType, SIGNAL(clicked(bool)),
            m_recordTypeSchedule, SLOT(setVisible(bool)));
    connect(m_queryRecordType, SIGNAL(clicked(bool)),
            m_recordTypeManual, SLOT(setVisible(bool)));
    connect(m_queryRecordType, SIGNAL(clicked(bool)),
            m_recordTypeAlarm, SLOT(setVisible(bool)));

    // camera
    // add a mean to clear select cameras
    m_queryCamerasEnabled = new QCheckBox(tr("&Camera(s)"), this);
    m_queryCameraList = new CameraListEdit(this);
    connect(m_queryCamerasEnabled, SIGNAL(clicked(bool)),
            m_queryCameraList, SLOT(setEnabled(bool)));

    m_queryTimeRange->setMinimumWidth(196);
    m_queryTimeBegin->setMinimumWidth(196);
    m_queryTimeEnd->setMinimumWidth(196);
    m_queryCameraList->setMinimumWidth(196);

    // query
    m_appendResults = new QCheckBox(tr("Append"), this);
    m_queryButton = new QPushButton(tr("Query"), this);

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(2, 4, 2, 0);

    layout->addWidget(timeRange, 0, 0, Qt::AlignRight);
    layout->addWidget(m_queryTimeRange, 0, 1, Qt::AlignLeft);

    layout->addWidget(m_queryTimeBeginEnabled, 1, 0, Qt::AlignRight);
    layout->addWidget(m_queryTimeBegin, 1, 1, Qt::AlignLeft);

    layout->addWidget(m_queryTimeEndEnabled, 2, 0, Qt::AlignRight);
    layout->addWidget(m_queryTimeEnd, 2, 1, Qt::AlignLeft);

    layout->addWidget(m_queryRecordType, 3, 0, 1, 2, Qt::AlignLeft);
    QHBoxLayout *recordtypelayout = new QHBoxLayout();
    recordtypelayout->setContentsMargins(0, 0, 2, 0);
    recordtypelayout->addWidget(m_recordTypeSchedule);
    recordtypelayout->addWidget(m_recordTypeManual);
    recordtypelayout->addWidget(m_recordTypeAlarm);
    layout->addLayout(recordtypelayout, 4, 0, 1, 2, Qt::AlignLeft);

    layout->addWidget(m_queryCamerasEnabled, 5, 0, Qt::AlignRight);
    layout->addWidget(m_queryCameraList, 5, 1, Qt::AlignLeft);

    QHBoxLayout *tmp = new QHBoxLayout();
    tmp->setContentsMargins(0, 0, 0, 0);
    tmp->addWidget(m_appendResults);
    tmp->addWidget(m_queryButton);
    layout->addItem(tmp, 6, 1, Qt::AlignRight);

    layout->setColumnStretch(0, 1);
    setLayout(layout);

    setFocusPolicy(Qt::StrongFocus);
    setFixedWidth(304);   // required by English texts
}

} // namespace Internal
} // namespace MEDIASOURCE
