#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QUuid>

#include <core/id.h>
#include <projectexplorer/constants.h>

#include "constants.h"
#include "kingvisiondialog.h"

namespace KingVision {
namespace Internal {

KingVisionDialog::KingVisionDialog(MediaSourceDialog *parent)
    : MediaSourceDialog(parent)
    , m_proxyAddr(new QLineEdit(this))
    , m_proxyPort(new QLineEdit(this))
{
    QLabel *proxyPortLabel = new QLabel(this);
    QLabel *proxyAddrLabel = new QLabel(this);

    QGroupBox *argumentBox = new QGroupBox(this);
    argumentBox->setTitle(tr("Connection Arguments"));

    proxyPortLabel->setText("存储服务代理端口*:");
    proxyAddrLabel->setText(tr("存储服务代理地址*:"));

    m_description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_gridlayout->addWidget(argumentBox, 2, 0, 4, 2);

    QGridLayout *argumentLayout = new QGridLayout();
    argumentBox->setLayout(argumentLayout);
    argumentLayout->addWidget(proxyAddrLabel, 0, 0, Qt::AlignRight);
    argumentLayout->addWidget(m_proxyAddr, 0, 1, Qt::AlignLeft);

    argumentLayout->addWidget(proxyPortLabel, 1, 0, Qt::AlignRight);
    argumentLayout->addWidget(m_proxyPort, 1, 1, Qt::AlignLeft);

    argumentLayout->addItem(new QSpacerItem(100, 250, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 0);
    argumentLayout->setColumnStretch(1,2);

}

void KingVisionDialog::aboutToCreateMediaSource()
{
    QVariantMap mediasource;
    QVariantMap argmenuts;

    mediasource.insert(QLatin1String(ProjectExplorer::Constants::MEDIASOURCE_CLASSID), QLatin1String(KingVision::Constants::MEDIASOURCE_CLASSID));

    QString id = QUuid::createUuid().toString();
    id.chop(1);
    id.remove(0,1);

    mediasource.insert(QLatin1String("ProjectExplorer.MetaData.Id"),id);
    mediasource.insert(QLatin1String("Name"), m_name->text());
    mediasource.insert(QLatin1String("Description"), m_description->toPlainText());
    mediasource.insert("Enable", 1);

    argmenuts.insert(QLatin1String("StorageProxyAddr"), m_proxyAddr->text());
    argmenuts.insert(QLatin1String("StorageProxyPort"), m_proxyPort->text());

    mediasource.insert(QLatin1String("Arguments") ,argmenuts);

    connect(this, &MS::MediaSourceDialog::createMediaSource,
            this, [mediasource]() { /*! 对应MediaSource 加接口*/ });

    emit createMediaSource(mediasource);
}

}
}
