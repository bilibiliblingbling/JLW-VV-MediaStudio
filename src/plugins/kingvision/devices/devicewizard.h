#ifndef DEVICEWIZARD_H
#define DEVICEWIZARD_H

#include <QStringListModel>
#include <QWizard>
#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QGridLayout;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListView;
class QRadioButton;
class QTextEdit;
QT_END_NAMESPACE

/*! @todo 这里暂时完成, 4.6
 * 简单完成,会在所有功能完成之后,集中修改一次界面
 * 使其达到预期效果.
 * 1.device model 如何定义更合理;
 * 2.导航栏效果达成,但切换页面时的逻辑挺烂的;
 * 3.QTextEdit 类被注册元信息时,获得的不是planToText(),所以这里用的时QLineEdit;
 *   后面会想一个折衷的办法,使其是QTextEdit, 并能保存客户输入的描述信息,
 *   上述问题会在后面模仿 "management center" 重新实现一下;
 * 4.确认元数据中的必填项目;
 * 5. "ipvalidator" 实现后,修改对应填写IP编辑框的输入格式;*/

#define DEVICE_SERIES_BQ 0
#define DEVICE_SERIES_ST 1

namespace KingVision {
namespace Internal {

class DeviceWizard : public QWizard
{
    Q_OBJECT

public:
    DeviceWizard();

public:
    enum {
        page_Base,
        page_BQ,
        page_ST
    };

    // QWizard interface
public:
    bool validateCurrentPage() override;

signals:
    void sendNewDeviceConfigInfo(const QVariantMap& arg);

private:
    QStringListModel *m_model;
};

////////////////////////////////////////////////////////////////////////////////

class BasePage : public QWizardPage
{
    Q_OBJECT

public:
    BasePage(QStringListModel *model);

    // QWizardPage interface
public:
    bool isComplete() const override;
    int nextId() const override;

private slots:
    void onModelChanged();
    void onIndexChanged(QModelIndex index);

private:
    uint m_deviceSeries; ///> 0 : BQXXX, 1 : STXXX
    bool m_deviceModelConfirm;

    QComboBox *m_typeComboBox;
    QComboBox *m_modelComboBox;

    QLineEdit *m_nameEdit;
    QLineEdit *m_descriptionEdit;

    QGroupBox *m_wayToConnectBox;

    QRadioButton *m_deviceToPlatformRaBtn;
    QRadioButton *m_platformToDeviceRaBtn;

    QListView *m_navigation;
    QStringListModel *m_model;
};

////////////////////////////////////////////////////////////////////////////////

class BQSeriesPage : public QWizardPage
{
    Q_OBJECT

public:
    BQSeriesPage(QStringListModel *model);

    // QWizardPage interface
public:
    int nextId() const override;

private slots:
    void onIndexChanged(QModelIndex index);

private:
    QLineEdit *m_deviceAddrEdit;
    QLineEdit *m_httpPortEdit;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_sipPortEdit;

    QListView *m_navigation;
    QStringListModel *m_model;
};

////////////////////////////////////////////////////////////////////////////////

class STSeriesPage : public QWizardPage
{
    Q_OBJECT

public:
    STSeriesPage(QStringListModel *model);

private slots:
    void onMulticasIsChecked();
    void onIndexChanged(QModelIndex index);

private:
    QLineEdit *m_deviceAddrEdit;
    QLineEdit *m_videoReceivePortEdit;
    QLineEdit *m_audioReceivePortEdit;
    QLineEdit *m_videoMulticastAddrEdit;
    QLineEdit *m_audioMulticastAddrEdit;

    QGroupBox *m_multicastBox;
    QCheckBox *m_multicastCheckBox;

    QListView *m_navigation;
    QStringListModel *m_model;
};

}// namespace Internal
}// namespace KingVision

#endif // DEVICEWIZARD_H
