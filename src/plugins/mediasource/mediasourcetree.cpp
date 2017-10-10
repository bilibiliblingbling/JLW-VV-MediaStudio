#include <QMenu>
#include <QTimer>

#include <core/action/actioncontainer.h>
#include <core/action/actionmanager.h>
#include <core/icore.h>

#include <projectexplorer/project.h>
#include <projectexplorer/sessionmanager.h>
#include <resource/resource.h>
#include <utils/algorithm.h>
#include <utils/tmplog.h>
#include <utils/wizard.h>

#include "constants.h"
#include "imediasourcefactory.h"
#include "mediasourcenode.h"
#include "mediasourcetree.h"
#include "mediasourcetreewidget.h"



namespace MediaSource {

namespace PE = ProjectExplorer;

namespace Internal {

class MediaSourceTreePrivate
{
public:
    ProjectExplorer::Node *m_currentNode;
    ProjectExplorer::Project *m_currentProject;
    QList<MediaSourceTreeWidget *> m_mediaSourceTreeWidgets;

    MediaSourceTreeWidget *m_focusMediaSourceTreeWidget;  // ForContextMenu
    Core::Context m_lastMediaSourceContext;

public:
    ProjectExplorer::VirtualFolderNode *m_virtualRootNode;  // parent node of toplevel items
    ProjectExplorer::VirtualFolderNode *m_rootNode;         // parent node of all mediasource classes

public:
    ProjectExplorer::MediaSource *m_mediasourceAdded;

public:
    QAction *m_newAction;

    QAction *m_browseAction;

    QAction *m_enableOrConnectAction;
    QAction *m_disableAction;

    QAction *m_renameAction;
    QAction *m_removeAction;

    QAction *m_propertyAction;
};
} // namespace Internal

/*!
    管理当前 Project 中的 MediaSource, 存储于 Node 中, 以树形式组织
    对外提供相关变化通知
 */

/*
 * media source structure:
 *
 *  MediaSourceRootNode
 *  |-- MediaSourceClass0
 *  |   |-- MediaSourceOfClass0
 *  |   |-- MediaSourceOfClass0
 *  |   `-- ...
 *  |
 *  |-- MediaSourceClass1
 *  |   |-- MediaSourceOfClass1
 *  |   |-- MediaSourceOfClass1
 *  |   `-- ...
 *  |
 *  `-- ...
 */

/*
 *  数据驱动方向
 *  Project -> BasicTree -> BasicModel -> BasicTreeView
 */

static MediaSourceTree *s_instance = 0;

MediaSourceTree::MediaSourceTree(QObject *parent)
    : ProjectExplorer::BasicTree(parent)
    , d(new Internal::MediaSourceTreePrivate())
{
    setObjectName(QStringLiteral("MediaSource.MediaSourceTree"));

    ////////////////////////////////////////////////////////////////////////////
    // node setup
    d->m_currentNode = 0;
    d->m_currentProject = 0;
    d->m_focusMediaSourceTreeWidget = 0;

    d->m_rootNode = new PE::VirtualFolderNode(Core::Id(Constants::MEDIASOURCETREE_ROOTNODE), tr("Media Source"));
    d->m_rootNode->setIcon(QIcon(Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, Resource::Constants::IDX_HOME)));

    d->m_virtualRootNode = new PE::VirtualFolderNode(Core::Id(Constants::MEDIASOURCETREE_VIRTUALROOTNODE));
    d->m_virtualRootNode->addSubFolderNode(d->m_rootNode);

    ////////////////////////////////////////////////////////////////////////////
    // register actions
    // media source related actions

    Core::Command *cmd = 0;
    Core::Context mediasourcetreecontext(Constants::C_MEDIASOURCE_TREEVIEW);
    Core::ActionContainer *mediasourcetreemenu
            = Core::ActionManager::createMenu(Constants::M_MEDIASOURCE_MEDIASOURCETREE);
    // TODO: create import/export media source file
    //

    Core::ActionContainer *mediasourceclassmenu
            = Core::ActionManager::createMenu(Constants::M_MEDIASOURCE_MEDIASOURCECLASS);

    mediasourceclassmenu->appendGroup(Constants::G_MEDIASOURCE_NEW);
    mediasourceclassmenu->addSeparator(Constants::G_MEDIASOURCE_NEW);
    d->m_newAction = new QAction(tr("New Media Source"), this);
    cmd = Core::ActionManager::registerAction(d->m_newAction,
                                              Constants::MEDIASOURCE_NEW);
    mediasourceclassmenu->addAction(cmd, Constants::G_MEDIASOURCE_NEW);

    Core::ActionContainer *mediasourcemenu
            = Core::ActionManager::createMenu(Constants::M_MEDIASOURCE_MEDIASOURCE);

    mediasourcemenu->appendGroup(Constants::G_MEDIASOURCE_BROWSE);
    mediasourcemenu->appendGroup(Constants::G_MEDIASOURCE_CONTROL);
    mediasourcemenu->appendGroup(Constants::G_MEDIASOURCE_MODIFY);
    mediasourcemenu->appendGroup(Constants::G_MEDIASOURCE_PROPERTY);

    mediasourcemenu->addSeparator(Constants::G_MEDIASOURCE_BROWSE);
    mediasourcemenu->addSeparator(Constants::G_MEDIASOURCE_CONTROL);
    mediasourcemenu->addSeparator(Constants::G_MEDIASOURCE_MODIFY);
    mediasourcemenu->addSeparator(Constants::G_MEDIASOURCE_PROPERTY);

    d->m_browseAction = new QAction(tr("Browse"), this);
    cmd = Core::ActionManager::registerAction(d->m_browseAction,
                                              Constants::MEDIASOURCE_BROWSE,
                                              mediasourcetreecontext);
    mediasourcemenu->addAction(cmd, Constants::G_MEDIASOURCE_BROWSE);

    d->m_enableOrConnectAction = new QAction(tr("Enable/Connect"), this);
    cmd = Core::ActionManager::registerAction(d->m_enableOrConnectAction,
                                              Constants::MEDIASOURCE_ENABLEORCONNECT,
                                              mediasourcetreecontext);
    mediasourcemenu->addAction(cmd, Constants::G_MEDIASOURCE_CONTROL);

    d->m_disableAction = new QAction(tr("Disable"), this);
    cmd = Core::ActionManager::registerAction(d->m_disableAction,
                                              Constants::MEDIASOURCE_DISABLE,
                                              mediasourcetreecontext);
    mediasourcemenu->addAction(cmd, Constants::G_MEDIASOURCE_CONTROL);

    d->m_renameAction = new QAction(tr("Rename"), this);
    cmd = Core::ActionManager::registerAction(d->m_renameAction,
                                              Constants::MEDIASOURCE_RENAME,
                                              mediasourcetreecontext);
    mediasourcemenu->addAction(cmd, Constants::G_MEDIASOURCE_MODIFY);

    d->m_removeAction = new QAction(tr("Remove"), this);
    cmd = Core::ActionManager::registerAction(d->m_removeAction,
                                              Constants::MEDIASOURCE_REMOVE,
                                              mediasourcetreecontext);
    mediasourcemenu->addAction(cmd, Constants::G_MEDIASOURCE_MODIFY);

    d->m_propertyAction = new QAction(tr("Property"), this);
    cmd = Core::ActionManager::registerAction(d->m_propertyAction,
                                              Constants::MEDIASOURCE_PROPERTY,
                                              mediasourcetreecontext);
    mediasourcemenu->addAction(cmd, Constants::G_MEDIASOURCE_PROPERTY);

    ////////////////////////////////////////////////////////////////////////////
    //connect Actions
    connect(d->m_newAction, &QAction::triggered,
            this, [this]() { createMediaSource(); });

    connect(d->m_browseAction, &QAction::triggered,
            this, [this]() { emitBrowseItem(currentNode()); });

    connect(d->m_enableOrConnectAction, &QAction::triggered,
            this, [this]() {
        if (PE::MediaSource *mediasource = currentMediaSource())
            mediasource->setEnabled(true);
    });
    connect(d->m_disableAction, &QAction::triggered,
            this, [this]() {
        if (PE::MediaSource *mediasource = currentMediaSource())
            mediasource->setEnabled(false);
    });

    //
    connect(d->m_renameAction, &QAction::triggered,
            this, [this]() {
        // TODO:
    });

    connect(d->m_removeAction, &QAction::triggered,
            this, [this]() {
        if (PE::MediaSource *mediasource = currentMediaSource())
            d->m_currentProject->removeMediaSource(mediasource);
    });

    connect(PE::SessionManager::instance(), &PE::SessionManager::startupProjectChanged,
            this, &MediaSourceTree::startupProjectChanged);

    startTimer(500);

    s_instance = this;
}

MediaSourceTree::~MediaSourceTree()
{
    s_instance = 0;

    delete d->m_virtualRootNode;
    delete d;
    d = 0;
}

MediaSourceTree *MediaSourceTree::instance()
{
    return s_instance;
}

/*!
 * \brief MediaSourceTree::rootNode
 * \return rootNode for rootIndex
 */
ProjectExplorer::FolderNode *MediaSourceTree::rootNode()
{
    return d->m_virtualRootNode;
}

ProjectExplorer::Node *MediaSourceTree::currentNode()
{
    return d->m_currentNode;
}

void MediaSourceTree::nodeChanged(PE::BasicTreeWidget *widget)
{
    Internal::MediaSourceTreeWidget *mediasourcetreewidget
            = dynamic_cast<Internal::MediaSourceTreeWidget *>(widget);
    if (mediasourcetreewidget && hasFocus(mediasourcetreewidget))
        updateFromMediaSourceTreeWidget(mediasourcetreewidget);
}

void MediaSourceTree::showContextMenu(PE::BasicTreeWidget *focus, const QPoint &globalPos, ProjectExplorer::Node *node)
{
    TM_MARK;
    Q_UNUSED(focus);
    // TODO: create context menu based on node
    QMenu *contextMenu = 0;

    if (!node)
        return;

    // update action status
    emit s_instance->aboutToShowContextMenu(node);

    if (node == s_instance->d->m_rootNode) {

    } else if (node->asNode<PE::VirtualFolderNode>()) {
        // create new media source
        contextMenu = Core::ActionManager::actionContainer(Constants::M_MEDIASOURCE_MEDIASOURCECLASS)->menu();
    } else if (Internal::MediaSourceNode *msn = node->asNode<Internal::MediaSourceNode>()) {
        // TODO:
        bool enabled = msn->metadata()->enabled();
        d->m_enableOrConnectAction->setEnabled(!enabled);
        d->m_disableAction->setEnabled(enabled);

        // browse, enable/disable, rename, remove, property
        contextMenu = Core::ActionManager::actionContainer(Constants::M_MEDIASOURCE_MEDIASOURCE)->menu();
    }

    if (contextMenu && contextMenu->actions().count() > 0) {
        contextMenu->popup(globalPos);
        // TODO:
    }
}

bool MediaSourceTree::selectableInTree(ProjectExplorer::Node *) const
{
    return true;
}

void MediaSourceTree::registerWidget(Internal::MediaSourceTreeWidget *widget)
{
    s_instance->d->m_mediaSourceTreeWidgets.append(widget);

    if (hasFocus(widget)) {
        s_instance->updateFromMediaSourceTreeWidget(widget);
    }
}

void MediaSourceTree::unregisterWidget(Internal::MediaSourceTreeWidget *widget)
{
    s_instance->d->m_mediaSourceTreeWidgets.removeOne(widget);
    if (hasFocus(widget)) {
        s_instance->updateFromNode(0);
    }
}

void MediaSourceTree::updateMediaSourceClasses()
{
    s_instance->d->m_rootNode->removeAllSubFolderNodes();

    auto mediasourcefactories = MediaSourceFactoryManager::factories();

    foreach (auto factory, mediasourcefactories) {
        ProjectExplorer::VirtualFolderNode *folder
                = new ProjectExplorer::VirtualFolderNode(factory->classId(), factory->className());
        folder->setIcon(QIcon(Resource::PixmapManager::inst().getPixmap(Resource::SmallIcons, Resource::Constants::IDX_CATEGORY)));
        s_instance->d->m_rootNode->addSubFolderNode(folder);
    }
}

void MediaSourceTree::emitBrowseItem(PE::Node *node)
{
    if (node) {
        Internal::MediaSourceNode *mediasourcenode = node->asNode<Internal::MediaSourceNode>();
        if (mediasourcenode && mediasourcenode->metadata())
            emit s_instance->browseMediaSource(mediasourcenode->metadata());
    }
}

void MediaSourceTree::timerEvent(QTimerEvent *)
{
    MediaSourceTree::updateMediaSourceNodes();
}

ProjectExplorer::MediaSource *MediaSourceTree::currentMediaSource()
{
    if (PE::Node *node = currentNode()) {
        Internal::MediaSourceNode *mediasourcenode = node->asNode<Internal::MediaSourceNode>();
        if (mediasourcenode)
            return mediasourcenode->metadata();
    }
    return 0;
}

bool MediaSourceTree::hasFocus(Internal::MediaSourceTreeWidget *widget)
{
    return (widget && ((widget->focusWidget() && widget->focusWidget()->hasFocus())
                       || (s_instance->d->m_focusMediaSourceTreeWidget == widget)));
}

void MediaSourceTree::updateFromMediaSourceTreeWidget(Internal::MediaSourceTreeWidget *widget)
{
    TM_MARK;
    update(widget->currentNode(), ProjectExplorer::SessionManager::startupProject());
}

void MediaSourceTree::updateFromNode(ProjectExplorer::Node *node)
{
    update(node, d->m_currentProject);
}

void MediaSourceTree::startupProjectChanged(ProjectExplorer::Project *currentStartupProject)
{
    update(0, currentStartupProject);
}

void MediaSourceTree::update(ProjectExplorer::Node *node, ProjectExplorer::Project *currentProject)
{
    bool projectChanged = (d->m_currentProject != currentProject);
    bool nodeChanged = (d->m_currentNode != node);

    if (projectChanged) {
        if (d->m_currentProject) {
            disconnect(d->m_currentProject, 0, this, 0);
            // TODO: clean old nodes;

            d->m_rootNode->removeAllSubFolderNodes();
            d->m_rootNode->removeAllNodes();
        }

        d->m_currentProject = currentProject;

        if (d->m_currentProject) {
            connect(d->m_currentProject, &ProjectExplorer::Project::mediaSourceAboutToBeAdded,
                    this, &MediaSourceTree::mediaSourceAboutToBeAdded);
            connect(d->m_currentProject, &ProjectExplorer::Project::mediaSourceAdded,
                    this, &MediaSourceTree::mediaSourceAdded);

            connect(d->m_currentProject, &ProjectExplorer::Project::mediaSourceAboutToBeRemoved,
                    this, &MediaSourceTree::mediaSourceAboutToBeRemoved);
            connect(d->m_currentProject, &ProjectExplorer::Project::mediaSourceRemoved,
                    this, &MediaSourceTree::mediaSourceRemoved);
        }

        if (d->m_currentProject) {
            foreach (auto mediasource, d->m_currentProject->mediaSources()) {
                mediaSourceAddedImpl(mediasource);
            }
        }
    }

    if (nodeChanged) {
        d->m_currentNode = node;
        emit currentNodeChanged(d->m_currentNode, currentProject);

        if (node) {
            // MediaSource 节点仅当确实变化后再更新
            if (PE::MediaSource *mediasource = qobject_cast<PE::MediaSource *>(node->metadata()))
                emit currentMediaSourceChanged(mediasource);
        }
    }

    if (projectChanged) {
        emit currentProjectChanged(d->m_currentProject);
    }
}

void MediaSourceTree::mediaSourceAboutToBeAdded(ProjectExplorer::MediaSource *source)
{
    d->m_mediasourceAdded = source;
}

void MediaSourceTree::mediaSourceAdded()
{
    if (d->m_mediasourceAdded && d->m_mediasourceAdded->isValid()) {
        mediaSourceAddedImpl(d->m_mediasourceAdded);
    }

    d->m_mediasourceAdded = 0;
}

void MediaSourceTree::mediaSourceAboutToBeRemoved(ProjectExplorer::MediaSource *source)
{
    if (!source || !source->isValid())
        return;

    ProjectExplorer::FolderNode *folder
            = Utils::findOr(d->m_rootNode->subFolderNodes(), 0,
                            [source](ProjectExplorer::FolderNode *f) -> bool { return (f->id() == source->classId()); });

    if (folder) {
        foreach (ProjectExplorer::Node *node, folder->childNodes()) {
            if (node && (node->id() == source->id())) {
                if (node == currentNode())
                    currentMediaSourceChanged(0);

                folder->removeNode(node);
                break;
            }
        }
    }
}

void MediaSourceTree::mediaSourceRemoved()
{
    // nothing to do
}

void MediaSourceTree::mediaSourceAddedImpl(ProjectExplorer::MediaSource *source)
{
    foreach (ProjectExplorer::FolderNode *folder, d->m_rootNode->subFolderNodes()) {
        if (folder->id() == source->classId()) {
            folder->addNode(new Internal::MediaSourceNode(source));
            break;
        }
    }
}

void MediaSourceTree::updateMediaSourceNodes()
{
    QVector<int> roles;
    roles += Qt::DecorationRole;
    foreach (PE::FolderNode *folder, d->m_rootNode->subFolderNodes()) {
        foreach (PE::Node *node, folder->childNodes()) {
            emit nodeUpdated(node, roles);
        }
    }
}

void MediaSourceTree::createMediaSource()
{
    TM_DBG << "这里创建";
    QList<QMultiMap<QString, QVariant> > pages;
    QMultiMap<QString, QVariant> page;
    QVariantList fields;
    QVariantMap field;

    //page 1
    page.insert("id", 1);
    page.insert("name", "基本信息");

    field.insert("id", 0001);
    field.insert("name", "名称");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("range", 0);/// 0 is no limit

    fields.append(field);
    field.clear();

    field.insert("id", 0002);
    field.insert("name", "描述");
    field.insert("type", Utils::FieldType::TextEdit);
    field.insert("requier", Utils::RequiredStatus::Optional);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("range", 0);/// 0 is no limit

    fields.append(field);
    field.clear();
    page.insert("fields", fields);
    fields.clear();
    pages.append(page);
    page.clear();

    //page2
    page.insert("id", 2);
    page.insert("name", "详细参数");

    field.insert("id", 0003);
    field.insert("name", "中心服务地址");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("range", 0);/// 0 is no limit

    fields.append(field);
    field.clear();

    field.insert("id", 0004);
    field.insert("name", "中心服务端口");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("range", 0);/// 0 is no limit

    fields.append(field);
    field.clear();

    field.insert("id", 0005);
    field.insert("name", "用户名");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("range", 0);/// 0 is no limit

    fields.append(field);
    field.clear();

    field.insert("id", 0006);
    field.insert("name", "密码");
    field.insert("type", Utils::FieldType::LineEdit);
    field.insert("requier", Utils::RequiredStatus::Required);
    field.insert("status", Utils::WidgetStauts::Normal);
    field.insert("range", 0);/// 0 is no limit

    fields.append(field);
    field.clear();
    page.insert("fields", fields);
    fields.clear();
    pages.append(page);
    page.clear();

    Utils::Wizard *wiz = new Utils::Wizard(pages, Core::ICore::dialogParent());
    wiz->exec();
}

} // namespace MediaSource
