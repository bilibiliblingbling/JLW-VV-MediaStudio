#include <QFont>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "videocontrlwidget.h"

namespace Monitor {
namespace Internal {

VideoContrlWidget::VideoContrlWidget(QWidget *parent)
    : QWidget(parent)
    , d(new VideoContrlWidgetPrivate())
{
    setBackground("#DCDCDC");
    //setMaximumHeight(60);
    widgetInit();
}

VideoContrlWidget::~VideoContrlWidget()
{
    delete d;
}

void VideoContrlWidget::widgetInit()
{
    /*
     * Utils::SkinButton *m_vBtnPlay;
    Utils::SkinButton *m_vBtnStop;
    Utils::SkinButton *m_vBtnPrev;
    Utils::SkinButton *m_vBtnNext;
    Utils::SkinButton *m_vBtnTimeIn;
    Utils::SkinButton *m_vBtnTimeOut;
    Utils::SkinButton *m_vBtnClose;
    //Utils::SkinButton *m_vBtnSpd1x;

    QComboBox *m_vSpdCombox;
    QCheckBox *m_vBtnCycle;
    QSlider *m_spdSlider;
     */

    QFont ft;
    ft.setPixelSize(12);

    d->m_vBtnPlay = new Utils::SkinButton(Resource::V_Play);
    d->m_vBtnStop = new Utils::SkinButton(Resource::V_Stop);
    d->m_vBtnPrev = new Utils::SkinButton(Resource::V_Prev);
    d->m_vBtnNext = new Utils::SkinButton(Resource::V_Next);

    d->m_vBtnSpeed = new Utils::SkinButton(Resource::Speed1x);
    d->m_vBtnClose = new Utils::SkinButton(Resource::V_Close);

    d->m_vBtnTimeIn  = new Utils::SkinButton(Resource::V_TimeIn);
    d->m_vBtnTimeOut = new Utils::SkinButton(Resource::V_TimeOut);

    d->m_spdSlider = new QSlider;
    d->m_spdSlider->setOrientation(Qt::Horizontal);
    d->m_spdSlider->setMinimumWidth(150);
    d->m_vCycleCheckBox = new QCheckBox;

    QSpacerItem *firstLineHeadSpacer = new QSpacerItem(10, 20, QSizePolicy::Expanding,
                                           QSizePolicy::Minimum);
    QSpacerItem *firstLineEndSpacer = new QSpacerItem(10, 20, QSizePolicy::Expanding,
                                           QSizePolicy::Minimum);
    QHBoxLayout *firstLineLayout = new QHBoxLayout;
    firstLineLayout->setAlignment(Qt::AlignRight);
    firstLineLayout->addItem(firstLineHeadSpacer);
    firstLineLayout->addWidget(d->m_vBtnSpeed);
    firstLineLayout->addWidget(d->m_spdSlider);
    firstLineLayout->addItem(firstLineEndSpacer);

    QHBoxLayout *secLineLayout = new QHBoxLayout;

    QSpacerItem *secLineHeadSpacer = new QSpacerItem(2, 20, QSizePolicy::Expanding,
                                                  QSizePolicy::Minimum);
    QHBoxLayout *secLineLayout1 = new QHBoxLayout;
    secLineLayout1->setSpacing(5);
    secLineLayout1->addWidget(d->m_vBtnPlay);
    secLineLayout1->addWidget(d->m_vBtnStop);

    QHBoxLayout *secLineLayout2 = new QHBoxLayout;
    secLineLayout2->setSpacing(5);
    secLineLayout2->addWidget(d->m_vBtnPrev);
    secLineLayout2->addWidget(d->m_vBtnNext);

    QHBoxLayout *secLineLayout3 = new QHBoxLayout;
    secLineLayout3->setSpacing(5);
    secLineLayout3->addWidget(d->m_vBtnTimeIn);
    secLineLayout3->addWidget(d->m_vBtnTimeOut);

    QHBoxLayout *secLineLayout4 = new QHBoxLayout;
    secLineLayout4->setSpacing(5);
    secLineLayout4->addWidget(d->m_vBtnSpeed);
    secLineLayout4->addWidget(d->m_spdSlider);

    QSpacerItem *secLineEndSpacer = new QSpacerItem(2, 20, QSizePolicy::Expanding,
                                                  QSizePolicy::Minimum);

    secLineLayout->setSpacing(10);
    secLineLayout->addItem(secLineHeadSpacer);
    secLineLayout->addLayout(secLineLayout1);
    secLineLayout->addLayout(secLineLayout2);
    secLineLayout->addLayout(secLineLayout3);
    secLineLayout->addLayout(secLineLayout4);
    secLineLayout->addItem(secLineEndSpacer);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(firstLineLayout);
    mainLayout->addLayout(secLineLayout);

}

void VideoContrlWidget::setBackground(QColor color)
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

} // namespace Internal
} // namespace Monitor
