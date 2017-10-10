#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QDialogButtonBox>

#include "mediasourcetree.h"
#include "mediasourcedialog.h"

namespace MediaSource {

MediaSourceDialog::MediaSourceDialog(QWidget *parent)
    : QDialog(parent)
    , m_name(new QLineEdit(this))
    , m_description(new QTextEdit(this))
    , m_gridlayout(new QGridLayout(this))
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    //mainLayout->setContentsMargins(8, 18, 8, 8);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumHeight(400);
    setMinimumWidth(370);

    QLabel *nameLabel = new QLabel(this);
    QLabel *descriptionLabel = new QLabel(this);

    nameLabel->setText(tr("Name:"));
    descriptionLabel->setText(tr("Description:"));

    m_gridlayout->addWidget(nameLabel, 0, 0, Qt::AlignRight);
    m_gridlayout->addWidget(m_name, 0, 1, Qt::AlignLeft);
    m_name->setMinimumWidth(255);
    m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    m_gridlayout->addWidget(descriptionLabel, 1, 0, Qt::AlignRight | Qt::AlignTop);
    m_gridlayout->addWidget(m_description, 1, 1, Qt::AlignLeft);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_gridlayout->addWidget(btnBox,6,1);

    mainLayout->addWidget(new QWidget(this), 1);
    mainLayout->addLayout(m_gridlayout);
    setLayout(mainLayout);

    connect(btnBox, SIGNAL(accepted()), this, SLOT(onFinshBtnClicked()));
    connect(btnBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void MediaSourceDialog::onFinshBtnClicked()
{
    aboutToCreateMediaSource();
}

} // namespace MediaSource

