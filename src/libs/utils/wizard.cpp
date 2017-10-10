#include <QCheckBox>
#include <QComboBox>
#include <QLayout>
#include <QLineEdit>
#include <QRadioButton>
#include <QTextEdit>

#include <QDebug>
#include "tmplog.h"

#include "wizard.h"

namespace Utils {
/*! e.g.:
 *
 *  Id(s) should be globally unique
 *
 *  if group id exists, the output map should keep it
 *
 **
 *  XML:
 *  <groups>
 *      <group name="" id="">
 *          <fields>
 *              <field id="">
 *                  <name>
 *                  <required_status/>
 *                  <type/>
 *                  <value_scope type="range/set">
 *                      <!--
 *                          <maximum>
 *                          <minimum>
 *                      -->
 *                      <!--
 *                          <values/>
 *                      -->
 *                  </value_scope>
 *                  <value/>
 *              </field>
 *          </fileds>
 *      </group>
 *  </groups>
 *
 **
 *  JSON
 *
 *
 ****** Input
 **
 *  QVariantList
 *
 *      group list, keep in order
 *      `-- group 0
 *          `-- group name
 *          `-- field list, keep in order
 *              `-- field 0
 *                  `-- id
 *                  `-- required status
 *                  `-- type
 *                  |   `--
 *                  `-- attribute name
 *                  `-- attribute value
 *
 ****** Output
 *  QVariantMap
 *
 *      map
 *      `-- attr_id -> attr_value
 *      `-- attr_id -> attr_value
 *      `-- attr_id -> attr_value
 *      `-- group_id -> group_values
 *          `-- attr_id -> attr_value
 *          `-- attr_id -> attr_value
 *          `-- attr_id -> attr_value
 */

/*! 动态加载存在很多问题, 有些目前无法解决.
 * 最终决定,使用静态加载向导页,
 * 根据条件适当的调整nextId, 使其达到要求
 * 预先设计的xml文件也无法满足业务需求, 重新设计
 * 添加 "特殊属性" tags,用以满足功能需求
 * 添加 "group" 类型组件, 递归调用, 实现groupbox的加载
 * 但是就某些触发功能,还要具体看看事件部分, 改进一下 */

/*! input
 * XML:
 *  <pages>
 *      <page name="" id="">
 *          <fields>
 *              <field id="">
 *                  <name>
 *                  <type/>
 *                  <widget_stauts/>
 *                  <required_status/>
 *                  <value_scope type="range/set">
 *                      <!--
 *                          <maximum>
 *                          <minimum>
 *                      -->
 *                      <!--
 *                          <values/>
 *                      -->
 *                  </value_scope>
 *                  <value/>
 *              </field>
 *          </fileds>
 *      </page>
 *  </pages>
 *
 * *************************************
 *
 *  output  (感觉KEY是ID的话存取都复杂,就算长远来看name也不会重复)
 * Map:
 *    key-> st:Required : Value
 *                          key->attr_name, value->attr_value
 *                          key->attr_name, value->attr_value
 *                          key->attr_name, value->attr_value
 * 根据requiredMap就可以满足最基本的设备创建
 *
 *    key-> st:Optional : Value
 *                          key->attr_name, value->attr_value
 *                          key->attr_name, value->attr_value
 *                          key->attr_name, value->attr_value
 * 根据optionalMap实现如描述等用户自定义设备属性
 * */

Wizard::Wizard(const QList<QMultiMap<QString, QVariant> > &pages, QWidget *parent)
    : QWizard(parent)
{
    setWizardStyle(WizardStyle::ModernStyle);
    //setOption(WizardOption::NoCancelButtonOnLastPage, true);
    setOption(WizardOption::CancelButtonOnLeft, true);
    setOption(WizardOption::DisabledBackButtonOnLastPage, true);
    setOption(WizardOption::NoBackButtonOnStartPage, true);
    setOption(WizardOption::NoBackButtonOnLastPage, true);

    initWizard(pages);
    setPages();
    setSubTitleFormat(Qt::TextFormat::RichText);
    setSideWidget(m_progress);
    sideWidget()->setMinimumWidth(100);

    connect(button(Wizard::NextButton), SIGNAL(clicked()), this, SLOT(onNextButtonClicked()));
    connect(button(Wizard::BackButton), SIGNAL(clicked()), this, SLOT(onBackBtttonClicked()));
    connect(this, SIGNAL(NextButtonClicked(int)), m_progress, SLOT(beforeNextButtonClicked(int)));
    connect(this, SIGNAL(BackButtonClicked(int)), m_progress, SLOT(beforeBackButtonClicked(int)));
    connect(this, SIGNAL(CurrentPageComplete(int)), m_progress, SLOT(onPageComplete(int)));
    connect(m_progress, SIGNAL(jumpPage(int))
                               ,this, SLOT(onProgressItemClicked(int)));
}

bool Wizard::validateCurrentPage()
{
    if (-1 == currentPage()->nextId()) {
        transferOutput();
    } else {
        currentPage()->nextId();
    }
    return true;
}

void Wizard::onNextButtonClicked()
{
    emit NextButtonClicked(currentId());
    QWizard::next();
}

void Wizard::onBackButtonClicked()
{
    emit BackButtonClicked(currentId());
    QWizard::back();
}



void Wizard::initWizard(const QList<QMultiMap<QString, QVariant> > &pages)
{
    ProgressItemList itemlst;
    int progressItemCount = 0;
    if(!pages.isEmpty()) {
        foreach (auto page, pages) {
            WizardPage *wPage = new WizardPage(page);
            m_pages.append(wPage);
            WizardProgressItem *item = new WizardProgressItem(page.value("name").toString(), progressItemCount);
            if ((itemlst.isEmpty()) || (itemlst.last()->text() != page.value("name").toString()))
                itemlst.append(item);
            progressItemCount++;
        }
        m_progress = new WizardProgress(itemlst);
    }
}

void Wizard::transferOutput()
{
    /// @todo 获取 regfields 的所有文件,
    /// 判断满足条件即可写入MAP传递给需要的对象
    /// 之前那种方式不太好,
    /// 最后问问王晓峰它想要啥东西把
    QVariantMap infoMap;
    /// @todo 一顿批哩啪啦写入操作....
    emit ValidateWizard(infoMap);
}

void Wizard::setPages()
{
    int pageId = 0;
    foreach (auto page, m_pages) {
        setPage(pageId, page);
        pageId++;
    }
}

void Wizard::onProgressItemClicked(int pos)
{
    TM_DBG << pos << currentId();
    int step = pos - currentId();
    if (step > 0) {
        for(int i = 0; i <= step; i++) {
            if (currentPage()->isComplete())
                next();
        }
    } else {
        for(int i = 0; i <= qAbs(step); i++) {
            back();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

static QStringList getRequierdFieldsName(const QVariantMap &map, const QString &key, const int &value)
{
    QStringList list;
    if (!map.isEmpty()) {
        QVariantList fields = map.value("fields").toList();
        if (!fields.isEmpty()) {
            foreach (auto field, fields) {
                QVariantMap fieldmap = field.toMap();
                if (fieldmap.value(key).toInt() == value) {
                    list.append(fieldmap.value("name").toString());
                }
            }
        }
    }
    return list;
}

WizardPage::WizardPage(const QVariantMap &page)
    : m_page(page)
{
    setTitle(page.value("name").toString());
    m_requierdFields = getRequierdFieldsName(page, "requier", RequiredStatus::Required);
    m_criticalFields = getRequierdFieldsName(page, "requier", RequiredStatus::Critical);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *widget = new QWidget(this);
    initPage(page, widget);
    layout->addWidget(widget);
    if (m_components.value(QStringLiteral("组播模式(合法范围:224.0.0.1 - 239.255.255.255)")))
        specialHandler(WizardSerial::Device);
}

void WizardPage::initializePage()
{
    // i don't want use it, it's way too complicated
}

void WizardPage::cleanupPage()
{
    //do nothing
}

bool WizardPage::validatePage()
{
    if(isComplete())
        return true;
    return false;
}

bool WizardPage::isComplete() const
{
    if (m_requierdFields.isEmpty() && m_criticalFields.isEmpty()) {
        return true;
    } else if (m_criticalFields.isEmpty()) {
        foreach (QString fieldname, m_requierdFields) {
            QLineEdit* le = dynamic_cast<QLineEdit *>(m_components.value(fieldname));
            if (le->text().isEmpty())
                return false;
        }
        return true;
    } else {
        QStringList needFields = m_requierdFields + m_criticalFields;
        foreach (QString fieldname, needFields) {
            if(field(fieldname).toString().isEmpty())
                return false;
        }
        return true;
    }
}

int WizardPage::nextId() const
{
    if(wizard()->currentId() != wizard()->pageIds().last()) {
        if(!m_criticalFields.isEmpty()) {
            /// 方便拓展其他逻辑的wizard 在这里加 else if
            if (hasComponents("设备型号"))
                return deviceNextId();
            return -1;
        } else {
            /// 这里一样加else if (有关键组件的向导中其他page的逻辑)
            if (hasFields("设备型号"))
                return -1;
            return wizard()->currentId() + 1;
        }
    } else {
        return -1;
    }
}

void WizardPage::initPage(const QVariantMap &map, QWidget *parent)
{
    if (!map.isEmpty()) {
        QVariantList fields = map.value("fields").toList();
        if (!fields.isEmpty()) {
            initFields(fields, parent);
        }
    }
}

void WizardPage::initFields(const QVariantList &fields, QWidget *parent)
{
    QGridLayout *layout = new QGridLayout(parent);
    int row = 0;
    foreach (auto field, fields) {
        QVariantMap fieldmap = field.toMap();
        QLabel *label = new QLabel();
        //label->setMaximumWidth(100);
        //label->setMinimumWidth(100);
        label->setAlignment(Qt::AlignRight | Qt::AlignTop);
        layout->addWidget(label, row, 0, Qt::AlignRight | Qt::AlignTop);
        switch (fieldmap.value("type").toInt()) {
        case FieldType::LineEdit :
        {
            QLineEdit *le = new QLineEdit(parent);
            if(isRequerid(fieldmap.value("requier").toInt())) {
                label->setText(QStringLiteral("*") + fieldmap.value("name").toString() + QStringLiteral(":"));
                connect(le, SIGNAL(textChanged(QString)), this, SLOT(onRequeridInfoComplete()));
                //connect(le, SIGNAL(editingFinished()), this, SLOT(onRequeridInfoComplete()));
            } else {
                label->setText(fieldmap.value("name").toString() + QStringLiteral(":"));
            }

            le->setMaximumWidth(260);
            setRange(le, fieldmap.value("rule").toInt());
            checkStatus(le, fieldmap.value("status").toInt());
            registerFieldWithName(le, fieldmap.value("name").toString());
            layout->addWidget(le, row, 1, Qt::AlignTop);
            m_components.insert(fieldmap.value("name").toString(), le);
            row++;
            break;
        }
        case FieldType::TextEdit :
        {
            QTextEdit *te = new QTextEdit(parent);
            if(isRequerid(fieldmap.value("requier").toInt())) {
                label->setText(QStringLiteral("*") + fieldmap.value("name").toString() + QStringLiteral(":"));
                connect(te, SIGNAL(textChanged()), this, SLOT(onRequeridInfoComplete()));
            } else {
                label->setText(fieldmap.value("name").toString() + QStringLiteral(":"));
            }

            te->setMaximumSize(260,68);
            setRange(te, fieldmap.value("rule").toInt());
            checkStatus(te, fieldmap.value("status").toInt());
            registerFieldWithName(te, fieldmap.value("name").toString(), "plainText", SIGNAL(QTextEdit::textChanged));
            layout->addWidget(te, row, 1, Qt::AlignTop);
            m_components.insert(fieldmap.value("name").toString(), te);
            row++;
            break;
        }
        case FieldType::ComboBox :
        {
            QComboBox *cob = new QComboBox(parent);
            if(isRequerid(fieldmap.value("requier").toInt())) {
                label->setText(QStringLiteral("*") + fieldmap.value("name").toString() + QStringLiteral(":"));
                connect(cob, SIGNAL(currentIndexChanged(int)), this, SLOT(onRequeridInfoComplete()));
            } else {
                label->setText(fieldmap.value("name").toString() + QStringLiteral(":"));
            }

            //cob->setMaximumWidth(260);
            setRange(cob, fieldmap.value("rule").toInt(), fieldmap.value("range"));
            checkStatus(cob, fieldmap.value("status").toInt());
            registerFieldWithName(cob, fieldmap.value("name").toString(), "currentData", SIGNAL(QComboBox::currentIndexChanged));
            layout->addWidget(cob, row, 1, Qt::AlignTop);
            m_components.insert(fieldmap.value("name").toString(), cob);
            row++;
            break;
        }
        case FieldType::CheckBox :
        {
            label->setVisible(false);
            QCheckBox *chb = new QCheckBox(parent);
            chb->setText(fieldmap.value("name").toString());
            if(isRequerid(fieldmap.value("requier").toInt())) {
                label->setText(QStringLiteral("*") + fieldmap.value("name").toString() + QStringLiteral(":"));
                connect(chb, SIGNAL(clicked()), this, SLOT(onRequeridInfoComplete()));
            } else {
                label->setText(fieldmap.value("name").toString() + QStringLiteral(":"));
            }

            setRange(chb, fieldmap.value("rule").toInt());
            checkStatus(chb, fieldmap.value("status").toInt());
            registerFieldWithName(chb, fieldmap.value("name").toString());
            layout->addWidget(chb, row, 0, 1, 2, Qt::AlignTop);
            m_components.insert(fieldmap.value("name").toString(), chb);
            row++;
            break;
        }
        case FieldType::RadioButton :
        {
            label->setVisible(false);
            QRadioButton *rb = new QRadioButton(parent);
            if(isRequerid(fieldmap.value("requier").toInt())) {
                label->setText(QStringLiteral("*") + fieldmap.value("name").toString() + QStringLiteral(":"));
                connect(rb, SIGNAL(clicked()), this, SLOT(onRequeridInfoComplete()));
            } else {
                label->setText(fieldmap.value("name").toString() + QStringLiteral(":"));
            }

            rb->setText(fieldmap.value("name").toString());
            setRange(rb, fieldmap.value("rule").toInt());
            checkStatus(rb, fieldmap.value("status").toInt());
            registerFieldWithName(rb, fieldmap.value("name").toString());
            layout->addWidget(rb, row, 0, 1, 2, Qt::AlignCenter | Qt::AlignTop);
            m_components.insert(fieldmap.value("name").toString(), rb);
            row++;
            break;
        }
        case FieldType::GroupBox :
        {
            label->setVisible(true);
            QGroupBox *gb = new QGroupBox(parent);
            if(isRequerid(fieldmap.value("requier").toInt())) {
                gb->setTitle(QStringLiteral("*") + fieldmap.value("name").toString() + QStringLiteral(":"));
                connect(gb, SIGNAL(toggled()), this, SLOT(onRequeridInfoComplete()));
            } else {
                gb->setTitle(fieldmap.value("name").toString());
            }

            //gb->setMaximumWidth(390);
            QVariantList subFields = fieldmap.value("subfields").toList();
            if (!subFields.isEmpty())
                initFields(subFields, gb);
            checkStatus(gb, fieldmap.value("status").toInt());
            //setRange(gb, fieldmap.value("rule").toInt());
            //registerFieldWithName(gb, fieldmap.value("name").toString());
            layout->addWidget(gb, row, 0, 1, 2, Qt::AlignTop);
            m_components.insert(fieldmap.value("name").toString(), gb);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    parent->setLayout(layout);
}

void WizardPage::specialHandler(const int &serial)
{
    switch (serial) {
    case WizardSerial::MediaSource :
        onMediaSourceSituation();
        break;
    case WizardSerial::Device :
    {
        onDeviceSituation();
        break;
    }
    default:
        break;
    }
}

bool Utils::WizardPage::hasComponents(const QString &componentname) const
{
    foreach (QString name, m_components.keys()) {
        if (componentname == name)
            return true;
    }
    return false;
}

bool WizardPage::hasFields(const QString &fieldname) const
{
    if (field(fieldname).isNull())
        return false;
    return true;
}

/// 想写个模板,在这里connect结果老是失败,等王晓峰回来问问!
bool WizardPage::isRequerid(const int &requiredSt) const
{
    if (requiredSt == RequiredStatus::Required || requiredSt == RequiredStatus::Critical) {
        return true;
    } else {
        return false;
    }
}

void WizardPage::checkStatus(QWidget *widget, const int &status)
{
    switch (status) {
    case WidgetStauts::Normal :
    {
        //do nothing
        break;
    }
    case WidgetStauts::Disable :
    {
        widget->setDisabled(true);
        break;
    }
    case WidgetStauts::Selected :
    {
        QComboBox *cb = dynamic_cast<QComboBox *>(widget);
        cb->setCurrentIndex(-1);
        break;
    }
    case WidgetStauts::TitleInvisible :
    {
        QGroupBox *gb = dynamic_cast<QGroupBox *>(widget);
        gb->setTitle("");
        break;
    }
    case WidgetStauts::Checked :
    {
        QRadioButton *rb = dynamic_cast<QRadioButton *>(widget);
        rb->setChecked(true);
        break;
    }
    default:
        break;
    }
}

void WizardPage::setRange(QWidget *widget, const int &inputRule, const QVariant &range)
{
    switch (inputRule) {
    case InputRule::Ordinary :
    {
        ///do nothing
        break;
    }
    case InputRule::IpInterval :
    {
        ///@todo set IpValidator
        break;
    }
    case InputRule::NumberInterval :
    {
        ///number range ,unused for now
        break;
    }
    case InputRule::CustomerInterval :
    {
        /// combobox,set range
        QVariantMap map = range.toMap();
        /// 就combobox 的自定义范围
        QComboBox *cb = dynamic_cast<QComboBox*>(widget);
        QMapIterator<QString, QVariant> it(map);
        while(it.hasNext()) {
            it.next();
            cb->addItem(it.key(), it.value().toInt());
        }
        break;
    }
    default:
        break;
    }
}

void WizardPage::registerFieldWithName(QWidget *widget, const QString &name, const char *property, const char *changedSignal)
{
    registerField(name, widget, property, changedSignal);
}

int WizardPage::deviceNextId() const
{
    if (hasComponents("设备型号")) {
        if (field("设备型号").toInt() > 0) {
            return wizard()->currentId() + field("设备型号").toInt();
        } else {
            return wizard()->currentId() + 1;
        }
    } else {
        return -1;
    }
}

void WizardPage::onMediaSourceSituation()
{
    //do nothing
}

void WizardPage::onDeviceSituation()
{
    QCheckBox *cb = findCmponent<QCheckBox>(QStringLiteral("组播模式(合法范围:224.0.0.1 - 239.255.255.255)"));
    connect(cb, SIGNAL(clicked(bool)), this, SLOT(resolveTheDeviceSubComponentsConnection(bool)));
}

void WizardPage::resolveTheDeviceSubComponentsConnection(bool isClicked)
{
    QGroupBox *gb = findCmponent<QGroupBox>(QStringLiteral("组播模式"));
    QMap<QString, QLineEdit* > leMap = findThisCmponentChildren<QLineEdit>(gb);
    if (isClicked) {
        QLineEdit* le = leMap.value("视频流组播地址");
        QLineEdit* le1 = leMap.value("音频流组播地址");
        le->setEnabled(true);
        le1->setEnabled(true);
    } else {
        QLineEdit* le = leMap.value("视频流组播地址");
        QLineEdit* le1 = leMap.value("音频流组播地址");
        le->setText("");
        le1->setText("");
        le->setEnabled(false);
        le1->setEnabled(false);
    }
}

void WizardPage::onRequeridInfoComplete()
{
    if(isComplete()) {
        nextId();
        wizard()->button(Wizard::NextButton)->setEnabled(true);
        wizard()->button(Wizard::FinishButton)->setEnabled(true);
        wizard()->button(Wizard::BackButton)->setEnabled(true);
        Wizard *wiz = dynamic_cast<Wizard *>(wizard());
        wiz->CurrentPageComplete(wizard()->currentId());
    }
}

////////////////////////////////////////////////////////////////////////////////

WizardProgress::WizardProgress(const ProgressItemList &progressList, QWidget *parent)
    : m_itemList(progressList)
{
    initProgress();
    connect(this, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))
            , this, SLOT(onItemChanged(QListWidgetItem*,QListWidgetItem*)));
}

void WizardProgress::onPageComplete(int pageId)
{
    ///当page0 完成时,进程条打开代表page1 的item
    foreach (auto item, m_itemList) {
        if((item->getCurrentItemPosition()) == (pageId + 1)) {
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item->setTextColor(Qt::GlobalColor::black);
        }
    }
}

void WizardProgress::beforeBackButtonClicked(int pageId)
{
    foreach (auto item, m_itemList) {
        item->getCurrentItemPosition() == pageId + 1;
        setItemSelected(item, true);
    }
}

void WizardProgress::beforeNextButtonClicked(int pageId)
{
    foreach (auto item, m_itemList) {
        item->getCurrentItemPosition() == pageId - 1;
        setItemSelected(item, true);
    }
}

void WizardProgress::initProgress()
{
    int count = 0;
    foreach (auto item, m_itemList) {
        addItem(item);
        if (0 == count) {
            item->setSelected(true);
        } else {
            item->setFlags(Qt::ItemIsSelectable);
            item->setTextColor(Qt::GlobalColor::gray);
            //item->setBackgroundColor(Qt::GlobalColor::gray);
        }
        count++;
    }
}

void WizardProgress::onItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)

    WizardProgressItem *currentItem = dynamic_cast<WizardProgressItem *>(current);
    int pos = currentItem->getCurrentItemPosition();
    emit jumpPage(pos);
}

////////////////////////////////////////////////////////////////////////////////

WizardProgressItem::WizardProgressItem(const QString &text, const int &itemPos)
    :m_itemPosition(itemPos)
{
    setText(text);
}

int WizardProgressItem::getCurrentItemPosition()
{
    return m_itemPosition;
}

} // namespace Utils

