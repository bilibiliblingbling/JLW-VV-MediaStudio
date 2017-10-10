#ifndef UTILS_WIZARD_H
#define UTILS_WIZARD_H

#include <QWizard>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>

#include "utils_global.h"

//Q_DECLARE_METATYPE(QWidget*)

namespace Utils {

enum WizardSerial {
      MediaSource
    , Device
};

enum FieldType {
      LineEdit
    , TextEdit
    , ComboBox
    , CheckBox
    , RadioButton
    , GroupBox
};

enum RequiredStatus {
      Unknown       = -1
    , Optional      = 0
    , Required
    , Critical
};

enum WidgetStauts {
      Normal
    , Disable
    , Selected
    , TitleInvisible
    , Checked
};

enum InputRule {
      Ordinary
    , IpInterval
    , NumberInterval
    , CustomerInterval
};

class WizardPage;
class WizardProgress;
class WizardProgressItem;

typedef QList<WizardPage* > PageList;
typedef QList<WizardProgressItem* > ProgressItemList;
typedef QMap<QString, QWidget* > QCompMap;

class MEDIASTUDIO_UTILS_EXPORT Wizard : public QWizard
{
    Q_OBJECT
public:
    Wizard(const QList<QMultiMap<QString, QVariant> > &pages, QWidget *parent);

    // QWizard interface
public:
     bool validateCurrentPage() Q_DECL_OVERRIDE;

public slots:
     void onNextButtonClicked();
     void onBackButtonClicked();

signals:
     void ValidateWizard(const QVariantMap &info);
     void CurrentPageComplete(int pageId);
     void NextButtonClicked(int pageId);
     void BackButtonClicked(int pageId);

private:
     void initWizard(const QList<QMultiMap<QString, QVariant> > &pages);
     void transferOutput();

private:
     void setPages();

private slots:
     void onProgressItemClicked(int pos);

private:
     PageList m_pages;
     WizardProgress *m_progress;
};

////////////////////////////////////////////////////////////////////////////////

class WizardPage : public QWizardPage
{
    Q_OBJECT
public:
    WizardPage(const QVariantMap &page);

public:
    // QWizardPage interface
    void initializePage() Q_DECL_OVERRIDE;
    void cleanupPage() Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;
    bool isComplete() const Q_DECL_OVERRIDE;
    int nextId() const Q_DECL_OVERRIDE;

public:
    /// 找到组件,当需要与其他组件产生特殊关系时(依赖,绑定时)
    template<typename T>
    T* findCmponent(const QString &fieldName)
    {
        foreach (QString component, m_components.keys()) {
            if (fieldName == component)
                return dynamic_cast<T*>(m_components.value(component));
            //QWidget* comp = dynamic_cast<QWidget*>(m_components.value(component));
        }
        return nullptr;
    }

    /// 针对GroupBox中的同类组件产生特殊关系
    template<typename T>
    QMap<QString, T*> findThisCmponentChildren(QGroupBox *gb)
    {
        QMap<QString, T*> childrenMap;
        foreach (QWidget* comp, m_components.values()) {
            if(comp->parent() == gb) {
                /// @todo need help!!!!!!!
                childrenMap.insert(m_components.key(comp), dynamic_cast<T*>(comp));
            }
        }
        return childrenMap;
    }

private:
    void initPage(const QVariantMap &page, QWidget* parent = 0);
    void initFields(const QVariantList &fields, QWidget* parent = 0);
    void specialHandler(const int &serial);

private:
    bool hasFields(const QString &fieldname) const;
    bool hasComponents(const QString &componentname) const;
    bool isRequerid(const int &requiredSt) const;

private:
    void checkStatus(QWidget *widget, const int &status);
    void setRange(QWidget *widget, const int &inputRule, const QVariant& range = 0);
    void registerFieldWithName(QWidget *widget, const QString &name,
                               const char *property = 0, const char *changedSignal = 0);

    /*! @brief 对于向导中所有非线形向导
     * 都应该有一个对应的nextIDfun,
     * 使其在wizardpage::nextId()中被调用,
     * 完成它自身的页面跳转规则;*/
private:
    int deviceNextId() const;

private:
    void onMediaSourceSituation();
    void onDeviceSituation();

private slots:
    void onRequeridInfoComplete();

private slots:
    void resolveTheDeviceSubComponentsConnection(bool isClicked);

private:
    QVariantMap m_page;
    QStringList m_requierdFields;
    QStringList m_criticalFields;
    QCompMap m_components;
};

////////////////////////////////////////////////////////////////////////////////

class WizardProgress : public QListWidget
{
    Q_OBJECT

public:
    WizardProgress(const ProgressItemList& progressList, QWidget* parent = 0);

signals:
    void jumpPage(int pos);

public slots:
    void onPageComplete(int pageId);

    void beforeBackButtonClicked(int pageId);
    void beforeNextButtonClicked(int pageId);

private:
    void initProgress();

private slots:
    void onItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
    ProgressItemList m_itemList;
};

////////////////////////////////////////////////////////////////////////////////

class WizardProgressItem : public QListWidgetItem
{
public:
    WizardProgressItem(const QString &text, const int &itemPos);

public:
    int getCurrentItemPosition();

private:
    int m_itemPosition;
};

} // namespace Utils

#endif // UTILS_WIZARD_H
