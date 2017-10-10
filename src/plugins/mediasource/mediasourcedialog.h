#ifndef MEDIASOURCEDIALOG_H
#define MEDIASOURCEDIALOG_H

#include <QDialog>

#include "mediasource_global.h"

QT_BEGIN_NAMESPACE
class QGridLayout;
class QLineEdit;
class QTextEdit;
class QDialogButtonBox;
QT_END_NAMESPACE

namespace MediaSource {

class MEDIASOURCE_EXPORT MediaSourceDialog : public QDialog
{
    Q_OBJECT

public:
    MediaSourceDialog(QWidget *parent = 0);

signals:
    void createMediaSource(const QVariantMap &args);

public slots:
    void onFinshBtnClicked();

private:
    virtual void aboutToCreateMediaSource() = 0;

protected:
    QLineEdit *m_name;
    QTextEdit *m_description;

    // 子类应该用这个布局器
    QGridLayout *m_gridlayout;
};

}
#endif // MEDIASOURCEDIALOG_H
