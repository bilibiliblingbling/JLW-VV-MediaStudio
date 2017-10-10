#include <QLabel>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QSpacerItem>

#include "kingvision/devices.h"

#include "camerapropertydialog.h"

namespace KingVision {
namespace Internal {

CameraPropertyDialog::CameraPropertyDialog(const Camera &camera, QWidget *parent)
    :QDialog (parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumHeight(400);


    QGridLayout *gridLayout = new QGridLayout();

    QLabel *nameLabel = new QLabel(this);
    QLabel *nameValue = new QLabel(this);
    QLabel *idLabel = new QLabel(this);
    QLabel *idValue = new QLabel(this);
    QLabel *codecLabel = new QLabel(this);
    QLabel *codecValue = new QLabel(this);
    QLabel *videoPortLabel = new QLabel(this);
    QLabel *videoPortValue = new QLabel(this);

    nameLabel->setText(tr("Name:"));
    idLabel->setText(tr("ID:"));
    codecLabel->setText(tr("Codec ID:"));
    videoPortLabel->setText(tr("Vedio Port:"));

    if(camera.isValid()) {
        QString name = camera.name();
        QString id = camera.id().toString();
        QString codecId = camera.codecId().toString();
        QString videoPort = QString::number(camera.videoPort());
        nameValue->setText(name);
        nameValue->setToolTip(name);

        idValue->setText(id);
        idValue->setToolTip(id);

        codecValue->setText(codecId);
        codecValue->setToolTip(codecId);

        videoPortValue->setText(videoPort);
        videoPortValue->setToolTip(videoPort);
    }

    gridLayout->addWidget(nameLabel, 0, 0, Qt::AlignRight);
    gridLayout->addWidget(nameValue, 0, 1, Qt::AlignLeft);
    gridLayout->setColumnStretch(1, 1);

    gridLayout->addWidget(idLabel, 1, 0, Qt::AlignRight);
    gridLayout->addWidget(idValue, 1, 1, Qt::AlignLeft);

    gridLayout->addWidget(codecLabel, 2, 0, Qt::AlignRight);
    gridLayout->addWidget(codecValue, 2, 1, Qt::AlignLeft);

    gridLayout->addWidget(videoPortLabel, 3, 0, Qt::AlignRight);
    gridLayout->addWidget(videoPortValue, 3, 1, Qt::AlignLeft);

    nameValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    idValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    codecValue->setTextInteractionFlags(Qt::TextSelectableByMouse);
    videoPortValue->setTextInteractionFlags(Qt::TextSelectableByMouse);

    gridLayout->addItem(new QSpacerItem(100, 250, QSizePolicy::Expanding, QSizePolicy::Expanding), 4, 0);

    QDialogButtonBox *okBtn = new QDialogButtonBox(QDialogButtonBox::Ok);
    gridLayout->addWidget(okBtn,5,1);

    gridLayout->setContentsMargins(0, 0, 0 , 0);
    gridLayout->setVerticalSpacing(8);
    gridLayout->setHorizontalSpacing(4);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(8, 18, 8, 8);
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(new QWidget(this), 1);

    setLayout(mainLayout);

    connect(okBtn, SIGNAL(accepted()), this, SLOT(accept()));
}

} // name space Internal
} // name space KingVision

