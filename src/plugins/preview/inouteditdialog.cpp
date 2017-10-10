#include <QFont>
#include <QDateTime>
#include <QDebug>

#include "inouteditdialog.h"

namespace Preview {
namespace Internal {

QString seconds_to_DHMS(quint64 duration)
{
  QString res;
  int seconds = (int) (duration % 60);
  duration /= 60;
  int minutes = (int) (duration % 60);
  duration /= 60;
  int hours = (int) (duration % 24);
  int days = (int) (duration / 24);
//  if((hours == 0)&&(days == 0))
//      return res.sprintf("%02d:%02d", minutes, seconds);
  if (days == 0)
      return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
  return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

InOutEditDialog::InOutEditDialog(QWidget *parent, PlayerData *playerData, PreviewVideoWidget *currentWidget)
    : QDialog(parent)
    , d(new InOutEditDialogPrivate())
    , m_playerData(NULL)
    , m_playBackData(NULL)
    , m_livePlayData(NULL)
{
    setFixedSize(420, 280);
    setWindowTitle(tr("In-out Edit Dialog"));
    m_playerData = playerData;

    m_playBackData = dynamic_cast<PlayBackData *>(playerData);
    if (!m_playBackData)
        m_livePlayData = dynamic_cast<LivePlayerData *>(playerData);

    m_currentWidget = currentWidget;
    memberwidgetsInit();

    setTime();
}

InOutEditDialog::~InOutEditDialog()
{
    delete d;
}

void InOutEditDialog::memberwidgetsInit()
{
    QFont ft;
    ft.setPixelSize(18);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setMargin(25);
    mainLayout->setSpacing(15);
    setLayout(mainLayout);

    QHBoxLayout *hTimeSpanLayout = new QHBoxLayout;
    d->m_timeSpanStaticLabel = new QLabel(tr("Time Span:"));
    d->m_timeSpanStaticLabel->setFont(ft);

    d->m_timeFromLabel = new QLabel(tr("16-03-10 18:06:15"));

    d->m_timeToStaticLabel = new QLabel(tr("To"));
    d->m_timeToStaticLabel->setFont(ft);

    d->m_timeToLabel = new QLabel(tr("16-03-10 18:06:35"));

    hTimeSpanLayout->addWidget(d->m_timeSpanStaticLabel);
    hTimeSpanLayout->addWidget(d->m_timeFromLabel);
    hTimeSpanLayout->addWidget(d->m_timeToStaticLabel);
    hTimeSpanLayout->addWidget(d->m_timeToLabel);
    hTimeSpanLayout->setSpacing(7);

    d->m_timeLengthStaticLabel = new QLabel(tr("Time Lenght:"));
    d->m_timeLengthStaticLabel->setFont(ft);
    d->m_timeLengthLabel = new QLabel(tr("00:01:10"));

    d->m_absoluteTimeRadioBtn = new QRadioButton(tr("Absolute Time"));
    d->m_absoluteTimeRadioBtn->setFont(ft);
    d->m_absoluteTimeRadioBtn->setChecked(true);
    connect(d->m_absoluteTimeRadioBtn, SIGNAL(clicked(bool)),
            this, SLOT(onAbsoluteTimeRadionChecked()));
    d->m_relativeTimeRadioBtn = new QRadioButton(tr("Relative Time"));
    d->m_relativeTimeRadioBtn->setFont(ft);
    connect(d->m_relativeTimeRadioBtn, SIGNAL(clicked(bool)),
            this, SLOT(onRelativeTimeRadioChecked()));

    d->m_setInTimeCheckBox = new QCheckBox(tr("Set in-Time"));
    d->m_setInTimeCheckBox->setFont(ft);
    connect(d->m_setInTimeCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(onSetInTimeCheckBoxChecked(bool)));
    d->m_setTimeInEdit = new QDateTimeEdit;
    d->m_setTimeInEdit->setReadOnly(true);

    d->m_setOutTimeCheckBox = new QCheckBox(tr("Set out-Time"));
    d->m_setOutTimeCheckBox->setFont(ft);
    connect(d->m_setOutTimeCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(onSetOutTimeCheckBoxChecked(bool)));
    d->m_setTimeOutEdit = new QDateTimeEdit;
    d->m_setTimeOutEdit->setReadOnly(true);
#if 0
    d->m_setTimeOutEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    d->m_setTimeOutEdit->setFrame(false);
#endif
    dialogButtonBoxInit();

    mainLayout->addLayout(hTimeSpanLayout, 0, 0, 1, 4);

    mainLayout->addWidget(d->m_timeLengthStaticLabel, 1, 0, 1, 1);
    mainLayout->addWidget(d->m_timeLengthLabel, 1, 1, 1, 3);

    mainLayout->addWidget(d->m_absoluteTimeRadioBtn, 2, 0, 1, 2, Qt::AlignHCenter);
    mainLayout->addWidget(d->m_relativeTimeRadioBtn, 2, 2, 1, 2, Qt::AlignLeft);

    mainLayout->addWidget(d->m_setInTimeCheckBox, 3, 0, 1, 1);
    mainLayout->addWidget(d->m_setTimeInEdit, 3, 1, 1, 3);

    mainLayout->addWidget(d->m_setOutTimeCheckBox, 4, 0, 1, 1);
    mainLayout->addWidget(d->m_setTimeOutEdit, 4, 1, 1, 3);

    mainLayout->addWidget(d->m_dlgBtnBox, 5, 0, 1, 4);

}

void InOutEditDialog::dialogButtonBoxInit()
{
    d->m_dlgBtnBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                          QDialogButtonBox::Cancel);

    connect(d->m_dlgBtnBox, &QDialogButtonBox::accepted,
            this, &InOutEditDialog::accept);
    connect(d->m_dlgBtnBox, &QDialogButtonBox::rejected,
            this, &InOutEditDialog::reject);

}

void InOutEditDialog::setTime()
{
    if (!m_playBackData)
        return; //TODO ; just for PlayBack

    QString cur = QDateTime::fromTime_t(m_playBackData->getBgnTime()).toString("yy-MM-dd hh:mm:ss");
    d->m_timeFromLabel->setText(cur);
    cur = QDateTime::fromTime_t(m_playBackData->getEndTime()).toString("yy-MM-dd hh:mm:ss");
    d->m_timeToLabel->setText(cur);

    cur = seconds_to_DHMS(m_playBackData->getTimeRange());
    d->m_timeLengthLabel->setText(cur);


    setTimeInEditDisPlayAbsoluteTime();
    setTimeOutEditDisPlayAbsoluteTime();
}
/*
 *dateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
  dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
  dateEdit->setDisplayFormat("yyyy.MM.dd");
*/
void InOutEditDialog::onAbsoluteTimeRadionChecked()
{
    setTimeInEditDisPlayAbsoluteTime();
    setTimeOutEditDisPlayAbsoluteTime();
}

void InOutEditDialog::onRelativeTimeRadioChecked()
{
    setTimeInEditDisPlayRelativeTime();
    setTimeOutEditDisPlayRelativeTime();
}

void InOutEditDialog::onSetInTimeCheckBoxChecked(bool isChecked)
{
    d->m_setTimeInEdit->setReadOnly(!isChecked);
}

void InOutEditDialog::onSetOutTimeCheckBoxChecked(bool isChecked)
{
    d->m_setTimeOutEdit->setReadOnly(!isChecked);
}

void InOutEditDialog::setTimeInEditDisPlayAbsoluteTime()
{
    if (!m_playBackData)
        return; //TODO ; just for PlayBack

    d->m_setTimeInEdit->clear();

    d->m_setTimeInEdit->setMinimumDateTime(QDateTime::fromTime_t(m_playBackData->getBgnTime()));
    d->m_setTimeInEdit->setMaximumDateTime(QDateTime::fromTime_t(m_playBackData->getEndTime()));
    d->m_setTimeInEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    if (m_playBackData->getTimeIn())
    {
        d->m_setInTimeCheckBox->setChecked(true);
        d->m_setTimeInEdit->setDateTime(QDateTime::fromTime_t(m_playBackData->getTimeIn()));
        d->m_setTimeInEdit->setReadOnly(false);
    }
}

void InOutEditDialog::setTimeInEditDisPlayRelativeTime()
{
    if (!m_playBackData)
        return; //TODO ; just for PlayBack

    d->m_setTimeInEdit->clear();

    QString cur = "00:00:00";
    d->m_setTimeInEdit->setMinimumTime(QTime::fromString(cur, "hh:mm:ss"));
    d->m_setTimeInEdit->setTime(QTime::fromString(cur, "hh:mm:ss"));
    cur = seconds_to_DHMS(m_playBackData->getTimeRange());
    d->m_setTimeInEdit->setMaximumTime(QTime::fromString(cur, "hh:mm:ss"));
    d->m_setTimeInEdit->setDisplayFormat("hh:mm:ss");

    if (m_playBackData->getTimeIn())
    {
        d->m_setInTimeCheckBox->setChecked(true);
        QString temp = seconds_to_DHMS(m_playBackData->getTimeIn() - m_playBackData->getBgnTime());
        d->m_setTimeInEdit->setTime(QTime::fromString(temp, "hh:mm:ss"));
        d->m_setTimeInEdit->setReadOnly(false);
    }
}

void InOutEditDialog::setTimeOutEditDisPlayAbsoluteTime()
{
    if (!m_playBackData)
        return; //TODO ; just for PlayBack

    d->m_setTimeOutEdit->clear();

    d->m_setTimeOutEdit->setMaximumDateTime(QDateTime::fromTime_t(m_playBackData->getEndTime()));
    d->m_setTimeOutEdit->setMinimumDateTime(QDateTime::fromTime_t(m_playBackData->getBgnTime()));
    d->m_setTimeOutEdit->setDateTime(QDateTime::fromTime_t(m_playBackData->getEndTime()));
    d->m_setTimeOutEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");

    if (m_playBackData->getTimeOut())
    {
        d->m_setOutTimeCheckBox->setChecked(true);
        d->m_setTimeOutEdit->setDateTime(QDateTime::fromTime_t(m_playBackData->getTimeOut()));
        d->m_setTimeOutEdit->setReadOnly(false);
    }
}

void InOutEditDialog::setTimeOutEditDisPlayRelativeTime()
{
    if (!m_playBackData)
        return; //TODO ; just for PlayBack

    d->m_setTimeOutEdit->clear();

    QString cur = "00:00:00";
    d->m_setTimeOutEdit->setMinimumTime(QTime::fromString(cur, "hh:mm:ss"));
    cur = seconds_to_DHMS(m_playBackData->getTimeRange());
    d->m_setTimeOutEdit->setMaximumTime(QTime::fromString(cur, "hh:mm:ss"));
    d->m_setTimeOutEdit->setTime(QTime::fromString(cur, "hh:mm:ss"));
    d->m_setTimeOutEdit->setDisplayFormat("hh:mm:ss");

    if (m_playBackData->getTimeOut())
    {
        d->m_setOutTimeCheckBox->setChecked(true);
        QString temp = seconds_to_DHMS(m_playBackData->getTimeOut() - m_playBackData->getBgnTime());
        d->m_setTimeInEdit->setTime(QTime::fromString(temp, "hh:mm:ss"));
        d->m_setTimeInEdit->setReadOnly(false);
    }
}

//QDateTime fromString(const QString &string, const QString &format)
void InOutEditDialog::accept()
{
    if (!m_playBackData)
        done(QDialog::Accepted); // TODO ; just for PlayBack

    if (d->m_setInTimeCheckBox->isChecked())
    {
        if (d->m_absoluteTimeRadioBtn->isChecked())
        {
            quint64 absSetInTime = d->m_setTimeInEdit->dateTime().toTime_t();
            qDebug() << "accept___ curTime and bgnTime :" << absSetInTime << m_playBackData->getBgnTime();
            if (absSetInTime != m_playBackData->getBgnTime() && absSetInTime != m_playBackData->getEndTime())
            {
                //m_playerData->m_timeIn = absSetInTime;
                m_playBackData->setTimeIn(absSetInTime); // record value
                m_currentWidget->setTimeIn(); // paint slider
            }
        }
        else
        {
            QTime time = d->m_setTimeInEdit->time();
            quint64 relSetInTime = (time.hour() * 3600 + time.minute() * 60 + time.second())
                    + m_playBackData->getBgnTime();
            //qDebug() << "accept___ relSetInTime :" << relSetInTime << " "<< m_playerData->m_bgnTime;
            if (relSetInTime != m_playBackData->getBgnTime() && relSetInTime != m_playBackData->getEndTime())
            {
                //m_playerData->m_timeIn = relSetInTime;
                m_playBackData->setTimeIn(relSetInTime);
                m_currentWidget->setTimeIn();
            }
        }
    }

    if (d->m_setOutTimeCheckBox->isChecked())
    {
        if (d->m_absoluteTimeRadioBtn->isChecked())
        {
            quint64 absTimeOut = d->m_setTimeOutEdit->dateTime().toTime_t();
            if (absTimeOut != m_playBackData->getBgnTime() && absTimeOut != m_playBackData->getEndTime())
            {
                //m_playerData->m_timeOut = absTimeOut;
                m_playBackData->setTimeOut(absTimeOut);
                m_currentWidget->setTimeOut();
            }
        }
        else
        {
            QTime timeOut = d->m_setTimeOutEdit->time();
            quint64 relTimeOut = (timeOut.hour() * 3600 + timeOut.minute() * 60 + timeOut.second())
                    + m_playBackData->getBgnTime();

            if (relTimeOut != m_playBackData->getBgnTime() && relTimeOut != m_playBackData->getEndTime())
            {
                //m_playerData->m_timeOut = relTimeOut;
                m_playBackData->setTimeOut(relTimeOut);
                m_currentWidget->setTimeOut();
            }
        }

    }

    done(QDialog::Accepted);
}

void InOutEditDialog::reject()
{
    done(QDialog::Rejected);
}

} // namespace Internal
} // namespace Preview
