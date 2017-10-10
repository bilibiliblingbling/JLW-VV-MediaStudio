#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QRadioButton>
#include <QSpacerItem>
#include <QTextEdit>

#include <utils/ipvalidator.h>

#include "devicewizard.h"

namespace KingVision {
namespace Internal {

DeviceWizard::DeviceWizard()
{
    QStringList pageList;
    pageList << tr("Base Information") << tr("Parameters Information");
    m_model = new QStringListModel(pageList);

    setPage(page_Base, new BasePage(m_model));
    setPage(page_BQ, new BQSeriesPage(m_model));
    setPage(page_ST, new STSeriesPage(m_model));

    setStartId(page_Base);
}

/*! @brief after finish button clicked
 * 从已注册的变量中获取配置信息用于新建设备
 * 确保信息中至少有model */

bool DeviceWizard::validateCurrentPage()
{
    QVariantMap configInfoMap;

    configInfoMap.insert("description",field("description"));
    configInfoMap.insert("name", field("name"));
    configInfoMap.insert("model", field("model"));

    if (currentId() == page_BQ) {
        if (-1 == currentPage()->nextId()) {
            configInfoMap.insert("deviceAddr", field("deviceAddrBQ"));
            configInfoMap.insert("httpPort", field("httpPort"));
            configInfoMap.insert("username", field("username"));
            configInfoMap.insert("password", field("password"));
            configInfoMap.insert("sipPort", field("sipPort"));

            emit sendNewDeviceConfigInfo(configInfoMap);
        }
    } else if (currentId() == page_ST) {
        if (-1 == currentPage()->nextId()) {
            configInfoMap.insert("deviceAddr", field("deviceAddrST"));
            configInfoMap.insert("videoPort", field("videoPort"));
            configInfoMap.insert("audioPort", field("audioPort"));
            configInfoMap.insert("muVideoPort", field("muVideoPort"));
            configInfoMap.insert("muAudioPort", field("muAudioPort"));

            emit sendNewDeviceConfigInfo(configInfoMap);
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

BasePage::BasePage(QStringListModel *model)
    : m_typeComboBox(new QComboBox())
    , m_modelComboBox(new QComboBox())
    , m_nameEdit(new QLineEdit())
    , m_descriptionEdit(new QLineEdit())
    , m_wayToConnectBox(new QGroupBox(tr("Connection Model")))
    , m_deviceToPlatformRaBtn(new QRadioButton(tr("Proactive connection platform")))
    , m_platformToDeviceRaBtn(new QRadioButton(tr("Platform Active Connectivity Devices")))
    , m_navigation(new QListView())
    , m_model(model)
{
    QLabel *typeLabel = new QLabel(tr("Device Type"));
    QLabel *modelLabel = new QLabel(tr("Device Model"));
    QLabel *nameLabel = new QLabel(tr("Device Name"));
    QLabel *descriptionLabel = new QLabel(tr("Device Description"));

    m_typeComboBox->addItem(tr("all type"));
    m_typeComboBox->setEnabled(false);

    m_modelComboBox->addItem("BQ-VIP6601S", DEVICE_SERIES_BQ);
    m_modelComboBox->addItem("BQ-VIP6901S", DEVICE_SERIES_BQ);
    m_modelComboBox->addItem("BQ-VIP6901A", DEVICE_SERIES_BQ);
    m_modelComboBox->addItem("BQ-VIP6911A", DEVICE_SERIES_BQ);
    m_modelComboBox->addItem("BQ-VIP6931S", DEVICE_SERIES_BQ);
    m_modelComboBox->addItem("BQ-VIP6921S", DEVICE_SERIES_BQ);
    m_modelComboBox->addItem("BQ-VIP6921A", DEVICE_SERIES_BQ);
    m_modelComboBox->addItem("Stream-TS", DEVICE_SERIES_ST);
    m_modelComboBox->addItem("Stream-TS-RTP", DEVICE_SERIES_ST);

    m_platformToDeviceRaBtn->setChecked(true);
    m_deviceToPlatformRaBtn->setEnabled(false);

    m_navigation->setModel(m_model);
    QModelIndex baseInfo = m_model->index(0);

    m_navigation->setCurrentIndex(baseInfo);
    m_navigation->setEnabled(false);
    m_navigation->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /*! @param key, value;
     * in QWizard, U can through "KEY" to find the "VALUE"
     * 将变量注册到 QWizard 中,
     * 成员都是QWidget的子类 */

    registerField("name", m_nameEdit);
    registerField("description", m_descriptionEdit);
    registerField("platformToDevice", m_platformToDeviceRaBtn);

    connect(m_modelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModelChanged()));
    connect(m_navigation, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexChanged(QModelIndex)));
    //布局
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(9, 9, 9, 15);

    gridLayout->addWidget(m_navigation, 0, 0, 9, 1);
    gridLayout->addWidget(typeLabel, 0, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(modelLabel, 1, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(nameLabel, 2, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(descriptionLabel, 3, 1, Qt::AlignHCenter | Qt::AlignTop);

    gridLayout->addWidget(m_typeComboBox, 0, 2);
    gridLayout->addWidget(m_modelComboBox, 1, 2);
    gridLayout->addWidget(m_nameEdit, 2, 2);
    gridLayout->addWidget(m_descriptionEdit, 3, 2, 2, 1);
    m_descriptionEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addWidget(m_wayToConnectBox, 5, 1, 2, 2);

    QGridLayout *groupLayout = new QGridLayout(m_wayToConnectBox);
    groupLayout->addWidget(m_platformToDeviceRaBtn, 0, 1);
    groupLayout->addWidget(m_deviceToPlatformRaBtn, 1, 1);

    QSpacerItem *spacer = new QSpacerItem(20,50, QSizePolicy::Maximum, QSizePolicy::Expanding);
    groupLayout->addItem(spacer, 2, 1);

    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 1);
    gridLayout->setRowStretch(2, 1);
    gridLayout->setRowStretch(3, 2);
    gridLayout->setRowStretch(4, 3);
    gridLayout->setRowStretch(5, 1);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 2);

    groupLayout->setColumnStretch(0, 1);
    groupLayout->setColumnStretch(1, 2);

}

bool BasePage::isComplete() const
{
    if (m_deviceModelConfirm) {
        return true;
    } else {
        return false;
    }
}

int BasePage::nextId() const
{
    if (m_deviceSeries) {
        return DeviceWizard::page_ST;
    } else {
        return DeviceWizard::page_BQ;
    }
}

void BasePage::onModelChanged()
{
    m_deviceSeries = m_modelComboBox->currentData().toInt();
    m_deviceModelConfirm = true;

    if (isComplete()) {
        QLineEdit *modeltext = new QLineEdit(m_modelComboBox->itemText(m_modelComboBox->currentIndex()));
        registerField("model", modeltext);

        wizard()->button(QWizard::NextButton)->setEnabled(true);
    }

    m_navigation->setEnabled(true);
    /*! @brief 返回上一页后 是否强制重新选择一次model*/
    //m_deviceModelConfirm = false;
}

void BasePage::onIndexChanged(QModelIndex index)
{
    if (index.row() == 1)
        wizard()->next();
    m_navigation->setCurrentIndex(m_model->index(0));
}

BQSeriesPage::BQSeriesPage(QStringListModel *model)
    : m_deviceAddrEdit(new QLineEdit())
    , m_httpPortEdit(new QLineEdit())
    , m_usernameEdit(new QLineEdit())
    , m_passwordEdit(new QLineEdit())
    , m_sipPortEdit(new QLineEdit())
    , m_navigation(new QListView())
    , m_model(model)
{
    QLabel *deviceAddrLabel = new QLabel(tr("Device Address"));
    QLabel *httpPortLabel = new QLabel(tr("HTTP Port"));
    QLabel *usernameLabel = new QLabel(tr("Username"));
    QLabel *passwordLabel = new QLabel(tr("Password"));
    QLabel *sipPortLabel = new QLabel(tr("SIP Port"));

    passwordLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

    m_navigation->setModel(m_model);
    QModelIndex bqSeries = m_model->index(1);
    m_navigation->setCurrentIndex(bqSeries);
    m_navigation->setEditTriggers(QAbstractItemView::NoEditTriggers);

    registerField("deviceAddrBQ", m_deviceAddrEdit);
    registerField("httpPort", m_httpPortEdit);
    registerField("username", m_usernameEdit);
    registerField("password", m_passwordEdit);
    registerField("sipPort", m_sipPortEdit);

    //布局
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(9, 9, 9, 15);

    //m_navigation->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    QValidator *validator = new Utils::IPv4Validator(this);
    m_deviceAddrEdit->setValidator(validator);

    gridLayout->addWidget(m_navigation, 0, 0, 7, 1);
    gridLayout->addWidget(deviceAddrLabel, 0, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(m_deviceAddrEdit, 0, 2);
    gridLayout->addWidget(httpPortLabel, 1, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(m_httpPortEdit, 1, 2);
    gridLayout->addWidget(usernameLabel, 2, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(m_usernameEdit, 2, 2);
    gridLayout->addWidget(passwordLabel, 3, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(m_passwordEdit, 3, 2);
    gridLayout->addWidget(sipPortLabel, 4, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(m_sipPortEdit, 4, 2);

    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(spacer, 6, 1, 1, 2);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 2);

    connect(m_navigation, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexChanged(QModelIndex)));
}

int BQSeriesPage::nextId() const
{

    return -1;
}

void BQSeriesPage::onIndexChanged(const QModelIndex index)
{
    if (!index.row())
        wizard()->back();
    m_navigation->setCurrentIndex(m_model->index(1));
}

////////////////////////////////////////////////////////////////////////////////

STSeriesPage::STSeriesPage(QStringListModel *model)
    : m_deviceAddrEdit (new QLineEdit())
    , m_videoReceivePortEdit (new QLineEdit())
    , m_audioReceivePortEdit (new QLineEdit())
    , m_videoMulticastAddrEdit (new QLineEdit())
    , m_audioMulticastAddrEdit (new QLineEdit())
    , m_multicastBox (new QGroupBox())
    //"Multicast Model(Legal Range: 224.0.0.1 - 239.255.255.255)"
    , m_multicastCheckBox(new QCheckBox(tr("组播模式:(有效范围: 224.0.0.1 - 239.255.255.255)")))
    , m_navigation(new QListView())
    , m_model(model)
{
    QLabel *deviceAddrLabel = new QLabel(tr("Device Address"));
    QLabel *videoReceivePortLabel = new QLabel(tr("Video Receive Port"));
    QLabel *audioReceivePortLabel = new QLabel(tr("Auido Receive Port"));
    QLabel *videoMulticastAddrLabel = new QLabel(tr("Video Multicast Address"));
    QLabel *audioMulticastAddrLabel = new QLabel(tr("Audio Multicast Address"));

    m_navigation->setModel(m_model);
    QModelIndex stSeries = m_model->index(1);
    m_navigation->setCurrentIndex(stSeries);
    m_navigation->setEditTriggers(QAbstractItemView::NoEditTriggers);

#if 1
    QRegExp regularExpression_ip("2(?:2[4-9]|3\\d)(?:\\.(?:25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]\\d?|0)){3}");
    QValidator *validator = new QRegExpValidator(regularExpression_ip, this);
#else
    QValidator *validator = new Utils::IPv4Validator(this);
#endif
    m_videoMulticastAddrEdit->setValidator(validator);
    m_audioMulticastAddrEdit->setValidator(validator);
    m_deviceAddrEdit->setValidator(validator);

    m_videoMulticastAddrEdit->setEnabled(false);
    m_audioMulticastAddrEdit->setEnabled(false);

    //布局
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(9, 9, 9, 15);

    gridLayout->addWidget(m_navigation, 0, 0, 6, 1);
    gridLayout->addWidget(deviceAddrLabel, 0, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(videoReceivePortLabel, 1, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(audioReceivePortLabel, 2, 1, Qt::AlignHCenter | Qt::AlignTop);
    gridLayout->addWidget(m_deviceAddrEdit, 0, 2, 1, 1);
    gridLayout->addWidget(m_videoReceivePortEdit, 1, 2, 1, 1);
    gridLayout->addWidget(m_audioReceivePortEdit, 2, 2, 1, 1);
    gridLayout->addWidget(m_multicastBox, 3, 1, 2, 2);

    QGridLayout *groupLayout = new QGridLayout(m_multicastBox);

    groupLayout->addWidget(m_multicastCheckBox, 0, 0, 1, 2);
    groupLayout->addWidget(videoMulticastAddrLabel, 1, 0);
    groupLayout->addWidget(m_videoMulticastAddrEdit, 1, 1);
    groupLayout->addWidget(audioMulticastAddrLabel, 2, 0);
    groupLayout->addWidget(m_audioMulticastAddrEdit, 2, 1);

    QSpacerItem *spacer = new QSpacerItem(50, 50, QSizePolicy::Minimum,QSizePolicy::Expanding);
    groupLayout->addItem(spacer, 3, 1);

    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 1);
    gridLayout->setRowStretch(2, 1);
    gridLayout->setRowStretch(3, 2);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 2);

    groupLayout->setColumnStretch(0, 1);
    groupLayout->setColumnStretch(1, 1);

    connect(m_multicastCheckBox, SIGNAL(clicked()), this, SLOT(onMulticasIsChecked()));
    connect(m_navigation, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexChanged(QModelIndex)));

}

void STSeriesPage::onMulticasIsChecked()
{
    if(m_multicastCheckBox->isChecked()) {
        m_videoMulticastAddrEdit->setEnabled(true);
        m_audioMulticastAddrEdit->setEnabled(true);
    } else {
        m_videoMulticastAddrEdit->setEnabled(false);
        m_audioMulticastAddrEdit->setEnabled(false);
    }
}

void STSeriesPage::onIndexChanged(QModelIndex index)
{
    if (!index.row())
        wizard()->back();
    m_navigation->setCurrentIndex(m_model->index(1));
}

} // namespace Internal
} // namespace KingVision
