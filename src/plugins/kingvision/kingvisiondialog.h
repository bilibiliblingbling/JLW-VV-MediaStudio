#ifndef KINGVISIONDIALOG_H
#define KINGVISIONDIALOG_H

#include <QDialog>

#include <mediasource/mediasourcedialog.h>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE

namespace KingVision {
namespace Internal {

namespace MS = MediaSource;

class KingVisionDialog : public MS::MediaSourceDialog
{
    Q_OBJECT
public:
    KingVisionDialog(MediaSourceDialog *parent = 0);

    // MediaSourceDialog interface
private:
    void aboutToCreateMediaSource() Q_DECL_OVERRIDE;

private:
    QLineEdit *m_proxyPort;
    QLineEdit *m_proxyAddr;
};

}
}

#endif // KINGVISIONDIALOG_H
