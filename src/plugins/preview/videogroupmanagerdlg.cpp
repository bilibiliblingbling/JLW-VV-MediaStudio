#include <resource/resource.h>
#include <QDebug>
#include "videogroupmanagerdlg.h"
#include "previewpanel.h"

namespace Preview {
namespace Internal{

/*!
    layout: QGridLayout
    +-------+-----------+--------+-------+-----------+
    | label | QComboBox | spacer | label | QComboBox |
    +-------+-----------+--------+-------+-----------+
    ..................................................
    all:8 rows, 5 columns
  */

VideoGroupManagerDlg::VideoGroupManagerDlg(QWidget *parent)
    : QDialog(parent)
    , d(new VideoGroupManagerDlgPrivate())
    //, m_videosInGroup(0)
{
    setWindowTitle(tr("video group manager dialog"));
    setFixedSize(410, 360);
    setContentsMargins(20, 10, 20, 10);
    m_videoWidgets = PreviewPanel::instance()->getVideoWidgets();
    m_videosCount = PreviewPanel::instance()->getWidgetsCount();

    m_colorVector  << Qt::lightGray << Qt::blue << Qt::red
                   << Qt::green << Qt::yellow << Qt::magenta
                   << Qt::cyan << "#A0522D" << Qt::darkGreen;

    dialogInit();
    setGroupComboxEnable();
    loadVideosInGroupRecords();
}

VideoGroupManagerDlg::~VideoGroupManagerDlg()
{
    delete d;
}

void VideoGroupManagerDlg::dialogInit()
{
    qint32 rows = 0;
    qint32 cols = 0;

    QVBoxLayout *dlgMainLayout = new QVBoxLayout;
    dlgMainLayout->setSpacing(26);
    setLayout(dlgMainLayout);

    QGridLayout *groupLayout = new QGridLayout;
    groupLayout->setMargin(5);
    groupLayout->setSpacing(8);
    dlgMainLayout->addLayout(groupLayout);

    for (int i = 0; i < d->WidgetsCount; ++i)
    {
        rows = i / 2;
        d->m_label = new QLabel(tr("win%1").arg(i+1));
        groupLayout->addWidget(d->m_label, rows, cols);
        d->m_groupCombox[i] = new QComboBox;
        QIcon icon = Resource::PixmapManager
                ::inst().getPixmap(Resource::GroupIcons, 0);
        d->m_groupCombox[i]->addItem(icon, tr("nogroup"));

        for (int c = 1; c < d->maxGroups; ++c) //9
        {
            icon = Resource::PixmapManager
                    ::inst().getPixmap(Resource::GroupIcons, c);
            d->m_groupCombox[i]->addItem(icon, tr("group%1").arg(c));
        }
        groupLayout->addWidget(d->m_groupCombox[i], rows, ++cols);

        if (cols == 1)
        {
            d->m_spacer = new QSpacerItem(16, 10, QSizePolicy::Expanding,
                                           QSizePolicy::Minimum);
            groupLayout->addItem(d->m_spacer, rows, ++cols);
        }
        ++cols;
        cols = cols % 5;
        //cols = (++cols) % 5;
    }

    dialogBtnsInit();
}

void VideoGroupManagerDlg::dialogBtnsInit()
{
    d->m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                           QDialogButtonBox::Cancel |
                                           QDialogButtonBox::Apply |
                                           QDialogButtonBox::Reset, this);
    layout()->addWidget(d->m_buttonBox);

    d->m_buttonBox->button(QDialogButtonBox::Apply)->setText(tr("yesToAll"));

    connect(d->m_buttonBox->button(QDialogButtonBox::Apply),
            &QAbstractButton::clicked,
            this, &VideoGroupManagerDlg::yesToAllInOneGroup);
    connect(d->m_buttonBox->button(QDialogButtonBox::Reset),
            &QAbstractButton::clicked,
            this, &VideoGroupManagerDlg::reset);

    connect(d->m_buttonBox, &QDialogButtonBox::accepted,
            this, &VideoGroupManagerDlg::accept);
    connect(d->m_buttonBox, &QDialogButtonBox::rejected,
            this, &VideoGroupManagerDlg::reject);
}

void VideoGroupManagerDlg::setGroupComboxEnable()
{
    for (int i = m_videosCount; i < d->WidgetsCount; ++i)
        d->m_groupCombox[i]->setEnabled(false);
}

void VideoGroupManagerDlg::loadVideosInGroupRecords()
{
#if 0
    m_videosInGroup = PreviewPanel::instance()->getVideosInGroupRecords();
    if (!m_videosInGroup->count())
        return;

    for (int i = 0; i < m_videosCount; ++i)
    {
        if (i < m_videosInGroup->count())
            d->m_groupCombox[i]->setCurrentIndex(m_videosInGroup->at(i));
        else //change layout
            d->m_groupCombox[i]->setCurrentIndex(0);
    }
#endif
    quint8 groupIndex;
    for (int i = 0; i < m_videosCount; ++i)
    {
        groupIndex = m_videoWidgets.at(i)->m_groupIndex;
        d->m_groupCombox[i]->setCurrentIndex(groupIndex);
    }
}

void VideoGroupManagerDlg::accept()
{
    qint8 index;

    for (int i = 0; i < m_videosCount; ++i)
    {
        index = d->m_groupCombox[i]->currentIndex();
        //m_videoWidgets.at(i)->m_groupIndex = index;
        //m_videosInGroup->replace(i, index);

        PreviewVideoWidget *w = m_videoWidgets.at(i);
        w->m_groupIndex = index;
        w->setVideoInGroup(m_colorVector[index]);
    }

    done(QDialog::Accepted);
}

void VideoGroupManagerDlg::reject()
{
    done(QDialog::Rejected);
}

void VideoGroupManagerDlg::yesToAllInOneGroup()
{
    for (int i = 0; i < m_videosCount; ++i)
        d->m_groupCombox[i]->setCurrentIndex(1);
}

void VideoGroupManagerDlg::reset()
{
    for (int i = 0; i < m_videosCount; ++i)
        d->m_groupCombox[i]->setCurrentIndex(0);
}

} // namespace Internal
} // namespace Preview
