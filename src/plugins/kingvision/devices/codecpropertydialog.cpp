#include <QLabel>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QSpacerItem>

#include "kingvision/devices.h"

#include "codecpropertydialog.h"

namespace KingVision {
namespace Internal {

CodecPropertyDialog::CodecPropertyDialog(const Codec &codec, QWidget *parent)
    : QDialog(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumHeight(400);

    QGridLayout *gridLayout = new QGridLayout();

    QLabel *idLabel = new QLabel(this);
    QLabel *idValue = new QLabel(this);
    QLabel *typeLabel = new QLabel(this);
    QLabel *typeValue = new QLabel(this);
    QLabel *modelLabel = new QLabel(this);
    QLabel *modelValue = new QLabel(this);
    QLabel *nameLabel = new QLabel(this);
    QLabel *nameValue = new QLabel(this);
    QLabel *descriptionLable = new QLabel(this);
    QLabel *descriptionValue = new QLabel(this);

    QLabel *addressLabel = new QLabel(this);
    QLabel *addressValue = new QLabel(this);
    QLabel *httpPortLabel = new QLabel(this);
    QLabel *httpPortValue = new QLabel(this);
    QLabel *usernameLabel = new QLabel(this);
    QLabel *usernameValue = new QLabel(this);
    QLabel *passwordLabel = new QLabel(this);
    QLabel *passwordValue = new QLabel(this);
    QLabel *sipPortLabel = new QLabel(this);
    QLabel *sipPortValue = new QLabel(this);

    idLabel->setText(tr("ID:*"));
    typeLabel->setText(tr("Type:*"));
    modelLabel->setText(tr("Model:*"));
    nameLabel->setText(tr("Name:*"));
    descriptionLable->setText(tr("Description:"));
    addressLabel->setText(tr("Address:*"));
    httpPortLabel->setText(tr("HTTP Port:*"));
    usernameLabel->setText(tr("Username:"));
    passwordLabel->setText(tr("Password:"));
    sipPortLabel->setText(tr("SIP Port:*"));

    if(codec.isValid()) {
        /*! @todo 未实现接口包括: type, model, address,
         * httpprot, username, password, sipPort */
        QString id = codec.id().toString();
        QString type = "视频编码器";
        QString model = "BQ-6931S";
        QString name = codec.name();
        QString description = codec.description();
        QString address = "192.168.x.xxx";
        QString httpPort = "80";
        QString username = "admin";
        QString password = "123456";
        QString sipPort = "5060";

        idValue->setText(id);
        idValue->setToolTip(id);
        typeValue->setText(type);
        typeValue->setToolTip(type);
        modelValue->setText(model);
        modelValue->setToolTip(model);
        nameValue->setText(name);
        nameValue->setToolTip(name);
        descriptionValue->setText(description);
        descriptionValue->setToolTip(description);

        addressValue->setText(address);
        addressValue->setToolTip(address);
        httpPortValue->setText(httpPort);
        httpPortValue->setToolTip(httpPort);
        usernameValue->setText(username);
        usernameValue->setToolTip(username);
        passwordValue->setText(password);
        passwordValue->setToolTip(password);

        sipPortValue->setText(sipPort);
    }

    gridLayout->addWidget(idLabel, 0, 0, Qt::AlignRight);
    gridLayout->addWidget(idValue, 0, 1, Qt::AlignLeft);

    gridLayout->addWidget(typeLabel, 1, 0, Qt::AlignRight);
    gridLayout->addWidget(typeValue, 1, 1, Qt::AlignLeft);

    gridLayout->addWidget(modelLabel, 2, 0, Qt::AlignRight);
    gridLayout->addWidget(modelValue, 2, 1, Qt::AlignLeft);

    gridLayout->addWidget(nameLabel, 3, 0, Qt::AlignRight);
    gridLayout->addWidget(nameValue, 3, 1, Qt::AlignLeft);

    gridLayout->addWidget(descriptionLable, 4, 0, Qt::AlignRight | Qt::AlignTop);
    gridLayout->addWidget(descriptionValue, 4, 1, 2, 1,Qt::AlignLeft | Qt::AlignTop);

    gridLayout->addWidget(addressLabel, 5, 0, Qt::AlignRight);
    gridLayout->addWidget(addressValue, 5, 1, Qt::AlignLeft);

    gridLayout->addWidget(httpPortLabel, 6, 0, Qt::AlignRight);
    gridLayout->addWidget(httpPortValue, 6, 1, Qt::AlignLeft);

    gridLayout->addWidget(usernameLabel, 7, 0, Qt::AlignRight);
    gridLayout->addWidget(usernameValue, 7, 1, Qt::AlignLeft);

    gridLayout->addWidget(passwordLabel, 8, 0, Qt::AlignRight);
    gridLayout->addWidget(passwordValue, 8, 1, Qt::AlignLeft);

    gridLayout->addWidget(sipPortLabel, 9, 0, Qt::AlignRight);
    gridLayout->addWidget(sipPortValue, 9, 1, Qt::AlignLeft);

    gridLayout->setColumnStretch(1, 1);

    idValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    typeValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    modelValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    nameValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    descriptionValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    addressValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    httpPortValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    usernameValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    passwordValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    sipPortValue->setTextInteractionFlags(Qt::TextSelectableByMouse);


    QDialogButtonBox *okBtn = new QDialogButtonBox(QDialogButtonBox::Ok);
    gridLayout->addItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Preferred), 10, 0);

    gridLayout->addWidget(okBtn,11,1);

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setVerticalSpacing(8);
    gridLayout->setHorizontalSpacing(4);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(8, 18, 8, 8);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(new QWidget(this), 1);

    setLayout(mainLayout);

    connect(okBtn, SIGNAL(accepted()), this, SLOT(accept()));
}


}   // namespace Internal
}   // namespace King Vision
