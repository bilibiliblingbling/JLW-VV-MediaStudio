#include <QMenu>

#include <core/icore.h>
#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>
#include <mediasource/constants.h>
#include <projectexplorer/basicnodes.h>
#include <projectexplorer/basictreewidget.h>
#include <resource/resource.h>
#include <utils/algorithm.h>
#include <utils/tmplog.h>
#include <utils/wizard.h>

#include "devices/camerapropertydialog.h"
#include "devices/codecpropertydialog.h"
#include "devices/devicewizard.h"

#include "constants.h"
#include "devices.h"
#include "devicetree.h"
#include "kingvisionnodes.h"
#include "mediasource.h"

namespace KingVision {
namespace Internal {

class DeviceTreePrivate
{
public:
    DeviceTreePrivate();

public:
    QAction *m_allStartRecording;
    QAction *m_allStopRecording;
    QAction *m_newDevice;

    QAction *m_removeDevice;
    QAction *m_property;
    QAction *m_query;

    QAction *m_startRecording;
    QAction *m_stopRecording;

public:
    ProjectExplorer::VirtualFolderNode *m_virtualRootNode;  // parent node of toplevel items
    ProjectExplorer::VirtualFolderNode *m_rootNode;         // parent node of all codec devices

public:
    ProjectExplorer::Node *m_currentNode;
    MediaSource *m_currentMediaSource;

public:
    Codec *m_codecAdded;
    Camera *m_cameraAdded;
};

static DeviceTree *s_devicetree = 0;

DeviceTree::DeviceTree(QObject *parent)
    : MS::DeviceTree(parent)
    , d(new DeviceTreePrivate())
{
    setObjectName(QStringLiteral("KingVision.DeviceTree"));

    //
    d->m_rootNode = new ProjectExplorer::VirtualFolderNode(Core::Id(Constants::KINGVISION_DEVICE_ROOTNODE), tr("Devices"));
    d->m_rootNode->setIcon(QIcon(Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, Resource::Constants::IDX_HOME)));

    d->m_virtualRootNode = new ProjectExplorer::VirtualFolderNode(Core::Id(Constants::KINGVISION_DEVICE_VIRTUALROOTNODE));
    d->m_virtualRootNode->addSubFolderNode(d->m_rootNode);

    // register actions
    Core::Command *cmd = 0;

    // actions
    Core::Context devicecontext(MS::Constants::C_MEDIASOURCE_DEVICETREEVIEW);
    Core::ActionContainer *devicetreemenu
            = Core::ActionManager::createMenu(Constants::M_KINGVISION_DEVICETREE);
    devicetreemenu->appendGroup(Constants::G_KINGVISION_RECORDING);
    devicetreemenu->appendGroup(Constants::G_KINGVISION_NEW);

    devicetreemenu->addSeparator(Constants::G_KINGVISION_RECORDING);
    devicetreemenu->addSeparator(Constants::G_KINGVISION_NEW);

    d->m_allStartRecording = new QAction(DeviceTree::tr("All Start Recording"), this);
    cmd = Core::ActionManager::registerAction(d->m_allStartRecording,
                                              Constants::KINGVISION_ALLSTARTRECORDING,
                                              devicecontext);
    devicetreemenu->addAction(cmd, Constants::G_KINGVISION_RECORDING);

    d->m_allStopRecording = new QAction(DeviceTree::tr("All Stop Recording"), this);
    cmd = Core::ActionManager::registerAction(d->m_allStopRecording,
                                              Constants::KINGVISION_ALLSTOPRECORDING,
                                              devicecontext);
    devicetreemenu->addAction(cmd, Constants::G_KINGVISION_RECORDING);

    d->m_newDevice = new QAction(DeviceTree::tr("New Device"), this);
    cmd = Core::ActionManager::registerAction(d->m_newDevice,
                                              Constants::KINGVISION_NEWDEVICE,
                                              devicecontext);
    devicetreemenu->addAction(cmd, Constants::G_KINGVISION_NEW);

    // device and camera
    Core::Command *propcmd = 0;
    Core::Command  *querycmd = 0;
    Core::ActionContainer *devicemenu
            = Core::ActionManager::createMenu(Constants::M_KINGVISION_DEVICE);
    devicemenu->appendGroup(Constants::G_KINGVISION_REMOVE);
    devicemenu->appendGroup(Constants::G_KINGVISION_QUERY);
    devicemenu->appendGroup(Constants::G_KINGVISION_PROPERTY);

    devicemenu->addSeparator(Constants::G_KINGVISION_REMOVE);
    devicemenu->addSeparator(Constants::G_KINGVISION_QUERY);
    devicemenu->addSeparator(Constants::G_KINGVISION_PROPERTY);

    d->m_removeDevice = new QAction(tr("Remove Device"), this);
    cmd = Core::ActionManager::registerAction(d->m_removeDevice,
                                              Constants::KINGVISION_REMOVEDEVICE,
                                              devicecontext);
    devicemenu->addAction(cmd, Constants::G_KINGVISION_REMOVE);

    d->m_query = new QAction(tr("Query Recordings"), this);
    querycmd = Core::ActionManager::registerAction(d->m_query,
                                                   Constants::KINGVISION_QUERYRECORDINGS,
                                                   devicecontext);
    devicemenu->addAction(querycmd, Constants::G_KINGVISION_QUERY);

    d->m_property = new QAction(tr("Property"), this);
    propcmd = Core::ActionManager::registerAction(d->m_property,
                                                  Constants::KINGVISION_RROPERTY,
                                                  devicecontext);
    devicemenu->addAction(propcmd, Constants::G_KINGVISION_PROPERTY);

    Core::ActionContainer *cameramenu
            = Core::ActionManager::createMenu(Constants::M_KINGVISION_DEVICE_CAMERA);
    cameramenu->appendGroup(Constants::G_KINGVISION_RECORDING);
    cameramenu->appendGroup(Constants::G_KINGVISION_QUERY);
    cameramenu->appendGroup(Constants::G_KINGVISION_PROPERTY);

    cameramenu->addSeparator(Constants::G_KINGVISION_RECORDING);
    cameramenu->addSeparator(Constants::G_KINGVISION_QUERY);
    cameramenu->addSeparator(Constants::G_KINGVISION_PROPERTY);

    d->m_startRecording = new QAction(tr("Start Recording"), this);
    cmd = Core::ActionManager::registerAction(d->m_startRecording,
                                              Constants::KINGVISION_STARTRECORDING,
                                              devicecontext);
    cameramenu->addAction(cmd, Constants::G_KINGVISION_RECORDING);

    d->m_stopRecording = new QAction(tr("Stop Recording"), this);
    cmd = Core::ActionManager::registerAction(d->m_stopRecording,
                                              Constants::KINGVISION_STOPRECORDING,
                                              devicecontext);
    cameramenu->addAction(cmd, Constants::G_KINGVISION_RECORDING);
    cameramenu->addAction(querycmd, Constants::G_KINGVISION_QUERY);
    cameramenu->addAction(propcmd, Constants::G_KINGVISION_PROPERTY);


    connect(d->m_query, &QAction::triggered,
            this, &DeviceTree::aboutToQueryCurrentCameras);

    connect(d->m_property, &QAction::triggered,
            this, &DeviceTree::displayDeviceProperties);

    connect(d->m_startRecording, &QAction::triggered,
            this, &DeviceTree::aboutToStartRecordingCurrentCamera);

    connect(d->m_stopRecording, &QAction::triggered,
            this, &DeviceTree::aboutToStopRecordingCurrentCamera);

    connect(d->m_newDevice, &QAction::triggered,
            this, &DeviceTree::displayDevicePraramWizard);

    connect(d->m_removeDevice, &QAction::triggered,
            this, &DeviceTree::removeDevice);

    connect(d->m_allStartRecording, &QAction::triggered,
            this, &DeviceTree::aboutToStartRecordingAll);

    connect(d->m_allStopRecording, &QAction::triggered,
            this, &DeviceTree::aboutToStopRecordingAll);

    s_devicetree = this;
}

DeviceTree::~DeviceTree()
{
    s_devicetree = 0;

    delete d->m_virtualRootNode;
    delete d;
    d = 0;
}

DeviceTree *DeviceTree::instance()
{
    return s_devicetree;
}

void DeviceTree::setCurrentMediaSource(PE::MediaSource *mediasource)
{
    if (MediaSource *kvms = qobject_cast<MediaSource *>(mediasource))
        update(0, kvms);
}

ProjectExplorer::FolderNode *DeviceTree::rootNode()
{
    return d->m_virtualRootNode;
}

ProjectExplorer::Node *DeviceTree::currentNode()
{
    return d->m_currentNode;
}

void DeviceTree::nodeChanged(ProjectExplorer::BasicTreeWidget *widget)
{
    if (widget)
        update(widget->currentNode(), d->m_currentMediaSource);
}

void DeviceTree::showContextMenu(PE::BasicTreeWidget *focus, const QPoint &globalPos, ProjectExplorer::Node *node)
{
    // TODO:
    Q_UNUSED(focus);

    if (!node)
        return;

    emit aboutToShowContextMenu(node);

    QMenu *contextMenu = 0;

    if (node == d->m_rootNode) {
        contextMenu = Core::ActionManager::actionContainer(Constants::M_KINGVISION_DEVICETREE)->menu();
    } else if (node->asNode<CodecNode>()) {
        contextMenu = Core::ActionManager::actionContainer(Constants::M_KINGVISION_DEVICE)->menu();
    } else if (node->asNode<CameraNode>()) {
        contextMenu = Core::ActionManager::actionContainer(Constants::M_KINGVISION_DEVICE_CAMERA)->menu();
    } else {
        TM_DBG << "invalid node:" << node->displayName().toStdString();
    }

    if (contextMenu && (contextMenu->actions().count() > 0)) {
        contextMenu->popup(globalPos);
        // TODO: may need to handle focus widget related stuff
    }
}

bool DeviceTree::dragableInTree(PE::Node *node) const
{
    return (node->asNode<CameraNode>());
}

void DeviceTree::aboutToQueryCurrentCameras()
{
    PE::Node *node = s_devicetree->currentNode();

    if (node && node->isValid()) {
        QList<Core::Id> cameras;

        if (CodecNode *codec = node->asNode<CodecNode>()) {
            foreach (auto camera, codec->subFolderNodes()) {
                Q_ASSERT(camera->asNode<CameraNode>());
                if (camera->isValid())
                    cameras.append(camera->id());
            }
        } else if (CameraNode *camera = node->asNode<CameraNode>()) {
            if (camera->isValid())
                cameras.append(camera->id());
        } else {
            TM_DBG << "invalid current node:" << node->displayName().toStdString();
            Q_ASSERT(0);
        }

        if (!cameras.isEmpty())
            emit aboutToQueryCameras(cameras);
    }
}

void DeviceTree::aboutToStartRecordingCurrentCamera()
{
    ProjectExplorer::Node *node = currentNode();
    if (node && node->isValid()) {
        if (CameraNode *camera = node->asNode<CameraNode>())
            d->m_currentMediaSource->startRecording(camera->id());
    } else {
        TM_DBG << "invalid current node:" << node->displayName().toStdString();
        Q_ASSERT(0); ///> 当前节点无效
    }
}

void DeviceTree::aboutToStopRecordingCurrentCamera()
{
    ProjectExplorer::Node *node = currentNode();
    if (node && node->isValid()) {
        if (CameraNode *camera = node->asNode<CameraNode>())
            d->m_currentMediaSource->stopRecording(camera->id());
    } else {
        TM_DBG << "invalid current node:" << node->displayName().toStdString();
        Q_ASSERT(0); ///> 当前节点无效
    }
}

void DeviceTree::aboutToStartRecordingAll()
{
    if (currentNode() == d->m_rootNode) {
        if (d->m_currentMediaSource)
            d->m_currentMediaSource->startRecordingAll();
    }
}

void DeviceTree::aboutToStopRecordingAll()
{
    if (currentNode() == d->m_rootNode) {
        if (d->m_currentMediaSource)
            d->m_currentMediaSource->stopRecordingAll();
    }
}

void DeviceTree::displayDeviceProperties()
{
    ProjectExplorer::Node *node = currentNode();
     if (node && node->isValid()) {
         QSharedPointer<QDialog> dialog;
         if (CameraNode *camera = node->asNode<CameraNode>()) {
             dialog.reset(new CameraPropertyDialog(*(camera->metadata()), Core::ICore::dialogParent()));
         } else if (CodecNode *codec = node->asNode<CodecNode>()) {
             dialog = QSharedPointer<QDialog>(new CodecPropertyDialog(*(codec->metadata()), Core::ICore::dialogParent()));
         }
         if (dialog)
             dialog->exec();
     }
}

void DeviceTree::displayDevicePraramWizard()
{
    TM_DBG << "这里创建";
    QList<QMultiMap<QString, QVariant> > pages;
    QMultiMap<QString, QVariant> page;
    QVariantList fields;
    QVariantMap field;
    QVariantList subFields;
    QVariantMap subField;
    QVariantMap range;
    //page 1
    page.insert("id", 1);
    page.insert("name", "基本信息");

    field.insert("id", 0001);
    field.insert("name", "设备类型");
    field.insert("type", Utils::FieldType::ComboBox );
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::CustomerInterval);

    range.insert("全部类型", 1);
    field.insert("range", range);/// 0 is no limit
    range.clear();

    fields.append(field);
    field.clear();

    field.insert("id", 0002);
    field.insert("name", "设备型号");
    field.insert("type", Utils::FieldType::ComboBox);
    field.insert("requier", Utils::RequiredStatus::Critical);
    field.insert("status", Utils::WidgetStauts::Selected);
    field.insert("rule", Utils::InputRule::CustomerInterval);

    range.insert("BQ-VIP6601S", 1);
    range.insert("BQ-VIP6901A", 1);
    range.insert("BQ-VIP6901S", 1);
    range.insert("BQ-VIP6911A", 1);
    range.insert("BQ-VIP6921A", 1);
    range.insert("BQ-VIP6921S", 1);
    range.insert("BQ-VIP6931S", 1);
    range.insert("Stream-TS", 2);
    range.insert("Stream-TS-RTP", 2);

    field.insert("range", range);
    range.clear();

    fields.append(field);
    field.clear();

    field.insert("id", 0003);
    field.insert("name", "设备名称");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0004);
    field.insert("name", "设备描述");
    field.insert("type", Utils::FieldType::TextEdit);
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0005);
    field.insert("name", "连接方式");
    field.insert("type", Utils::FieldType::GroupBox);
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    subField.insert("id", 1001);
    subField.insert("name", "平台主动连接设备");
    subField.insert("type", Utils::FieldType::RadioButton);
    subField.insert("requier", Utils::RequiredStatus::Optional);
    subField.insert("status", Utils::WidgetStauts::Checked);
    subField.insert("rule", Utils::InputRule::Ordinary);
    subField.insert("range", 0);

    subFields.append(subField);
    subField.clear();

    subField.insert("id", 1002);
    subField.insert("name", "设备主动连接平台");
    subField.insert("type", Utils::FieldType::RadioButton);
    subField.insert("requier", Utils::RequiredStatus::Optional);
    subField.insert("status", Utils::WidgetStauts::Disable);
    subField.insert("rule", Utils::InputRule::Ordinary);
    subField.insert("range", 0);

    subFields.append(subField);
    subField.clear();

    field.insert("subfields", subFields);
    subFields.clear();

    fields.append(field);
    field.clear();

    page.insert("fields", fields);
    fields.clear();
    pages.append(page);
    page.clear();

    //page2.1
    page.insert("id", 2);
    page.insert("name", "连接参数");

    field.insert("id", 0001);
    field.insert("name", "设备地址");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0002);
    field.insert("name", "HTTP端口");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0003);
    field.insert("name", "用户名");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0004);
    field.insert("name", "密码");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0005);
    field.insert("name", "SIP端口");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();
    page.insert("fields", fields);
    fields.clear();
    pages.append(page);
    page.clear();

    //page2.2
    page.insert("id", 3);
    page.insert("name", "连接参数");

    field.insert("id", 0001);
    field.insert("name", "设备地址");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0002);
    field.insert("name", "视频流接收端口");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0003);
    field.insert("name", "音频流接收端口");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    fields.append(field);
    field.clear();

    field.insert("id", 0004);
    field.insert("name", "组播模式");
    field.insert("type", Utils::FieldType::GroupBox);
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::TitleInvisible);
    field.insert("rule", Utils::InputRule::Ordinary);
    field.insert("range", 0);

    subField.insert("id", 1001);
    subField.insert("name", "组播模式(合法范围:224.0.0.1 - 239.255.255.255)");
    subField.insert("type", Utils::FieldType::CheckBox);
    subField.insert("requier", Utils::RequiredStatus::Optional);
    subField.insert("status", Utils::WidgetStauts::Normal);
    subField.insert("rule", Utils::InputRule::Ordinary);
    subField.insert("range", 0);

    subFields.append(subField);
    subField.clear();

    subField.insert("id", 1002);
    subField.insert("name", "视频流组播地址");
    subField.insert("type", Utils::FieldType::LineEdit);
    subField.insert("requier", Utils::RequiredStatus::Optional);
    subField.insert("status", Utils::WidgetStauts::Disable);
    subField.insert("rule", Utils::InputRule::Ordinary);
    subField.insert("range", 0);

    subFields.append(subField);
    subField.clear();

    subField.insert("id", 1003);
    subField.insert("name", "音频流组播地址");
    subField.insert("type", Utils::FieldType::LineEdit);
    subField.insert("requier", Utils::RequiredStatus::Optional);
    subField.insert("status", Utils::WidgetStauts::Disable);
    subField.insert("rule", Utils::InputRule::Ordinary);
    subField.insert("range", 0);

    subFields.append(subField);
    subField.clear();

    field.insert("subfields", subFields);
    subFields.clear();

    fields.append(field);
    field.clear();

    page.insert("fields", fields);
    fields.clear();

    pages.append(page);
    page.clear();

    Utils::Wizard *wiz = new Utils::Wizard(pages, Core::ICore::dialogParent());
    wiz->exec();
}

void DeviceTree::removeDevice()
{
    ProjectExplorer::Node *node = currentNode();
    if (node && node->isValid()) {
       if (CodecNode *codecnode = node->asNode<CodecNode>()) {
           Codec *codec = codecnode->metadata();
           /*! @todo mediasource 没有对应删除媒体源接口, 暂时只删节点*/
       }
    }
}

void DeviceTree::update(ProjectExplorer::Node *node, MediaSource *currentMediaSource)
{
    bool mediaSourceChanged = (d->m_currentMediaSource != currentMediaSource);
    bool nodeChanged = (d->m_currentNode != node);

    if (mediaSourceChanged) {
        if (d->m_currentMediaSource) {
            disconnect(d->m_currentMediaSource, 0, this, 0);

            // clean old devices
            codecsAboutToBeCleared();
        }

        d->m_currentMediaSource = currentMediaSource;

        if (d->m_currentMediaSource) {
            connect(d->m_currentMediaSource, &MediaSource::codecAboutToBeAdded,
                    this, &DeviceTree::codecAboutToBeAdded);
            connect(d->m_currentMediaSource, &MediaSource::codecAdded,
                    this, &DeviceTree::codecAdded);

            connect(d->m_currentMediaSource, &MediaSource::codecAboutToBeRemoved,
                    this, &DeviceTree::codecAboutToBeRemoved);
            connect(d->m_currentMediaSource, &MediaSource::codecRemoved,
                    this, &DeviceTree::codecRemoved);

            connect(d->m_currentMediaSource, &MediaSource::codecsAboutToBeCleared,
                    this, &DeviceTree::codecsAboutToBeCleared);
            connect(d->m_currentMediaSource, &MediaSource::codecsCleared,
                    this, &DeviceTree::codecsCleared);

            connect(d->m_currentMediaSource, &MediaSource::cameraAboutToBeAdded,
                    this, &DeviceTree::cameraAboutToBeAdded);
            connect(d->m_currentMediaSource, &MediaSource::cameraAdded,
                    this, &DeviceTree::cameraAdded);

            connect(d->m_currentMediaSource, &MediaSource::destroyed,
                    this, [this]() { update(0, 0); });

            foreach (auto codec, d->m_currentMediaSource->codecs()) {
                codecAddedImpl(codec);
            }

            foreach (auto camera, d->m_currentMediaSource->cameras()) {
                cameraAddedImpl(camera);
            }
        }
    }

    if (nodeChanged) {
        d->m_currentNode = node;
        emit currentNodeChanged(d->m_currentNode, currentMediaSource);
    }
}

void DeviceTree::codecAboutToBeAdded(Codec *codec)
{
    d->m_codecAdded = codec;
}

void DeviceTree::codecAdded()
{
    if (d->m_codecAdded && d->m_codecAdded->isValid())
        codecAddedImpl(d->m_codecAdded);

    d->m_codecAdded = 0;
}

void DeviceTree::cameraAboutToBeAdded(Camera *camera)
{
    d->m_cameraAdded = camera;
}

void DeviceTree::cameraAdded()
{
    if (d->m_cameraAdded && d->m_cameraAdded->isValid())
        cameraAddedImpl(d->m_cameraAdded);

    d->m_cameraAdded = 0;
}

void DeviceTree::codecAboutToBeRemoved(Codec *codec)
{
    if (codec && codec->isValid()) {
        ProjectExplorer::FolderNode *folder
                = Utils::findOr(d->m_rootNode->subFolderNodes(), 0,
                                [codec](ProjectExplorer::FolderNode *f) -> bool { return (f->id() == codec->id()); });
        if (folder) {
            d->m_rootNode->removeSubFolderNode(folder);
        }
    }
}

void DeviceTree::codecRemoved()
{
    // nothing to do
}

void DeviceTree::codecsAboutToBeCleared()
{
    d->m_rootNode->removeAllSubFolderNodes();
    d->m_rootNode->removeAllNodes();
}

void DeviceTree::codecsCleared()
{
    // nothing to do
}

void DeviceTree::codecAddedImpl(Codec *codec)
{
    d->m_rootNode->addSubFolderNode(new CodecNode(codec));
}

void DeviceTree::cameraAddedImpl(Camera *camera)
{
    foreach (auto folder, d->m_rootNode->subFolderNodes()) {
        CodecNode *codec = folder->asNode<CodecNode>();
        if (codec->metadata()->id() == camera->codecId()) {
            codec->addSubFolderNode(new CameraNode(camera));
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

DeviceTreePrivate::DeviceTreePrivate()
    : m_virtualRootNode(0)
    , m_rootNode(0)
    , m_currentNode(0)
    , m_currentMediaSource(0)
    , m_codecAdded(0)
    , m_cameraAdded(0)
{
}

} // namespace Internal
} // namespace KingVision
