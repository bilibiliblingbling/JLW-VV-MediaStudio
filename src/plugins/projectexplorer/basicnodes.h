#ifndef BASIC_NODES_H
#define BASIC_NODES_H

#include <QIcon>

#include <core/id.h>

#include "projectexplorer_global.h"

namespace ProjectExplorer {

class MetaData;
class Node;
class FolderNode;

class PROJECTEXPLORER_EXPORT Node
{
public:
    explicit Node();
    virtual ~Node();

public:
    virtual Core::Id id() const;
    virtual QString displayName() const;
    virtual QString tooltip() const;
    virtual bool enabled() const;
    virtual QIcon icon() const;

public:
    virtual MetaData *metadata() const;

public:
    bool isValid() const;

    FolderNode *parentFolderNode() const;

    template<typename T>
    T *asNode()
    {
        return dynamic_cast<T *>(this);
    }

public:
    virtual qint32 priority() const;

public:
    void emitNodeUpdated();

protected:
    void setParentFolderNode(FolderNode *folder);

protected:
    void emitNodeHiddenStateAboutToChange();
    void emitNodeHiddenStateChanged();

    void emitNodeSortKeyAboutToChange();
    void emitNodeSortKeyChanged();

private:
    FolderNode *m_folderNode;

private:
    friend class FolderNode;
};

////////////////////////////////////////////////////////////////////////////////

class PROJECTEXPLORER_EXPORT FolderNode : public Node
{
public:
    FolderNode();
    virtual ~FolderNode();

public:
    void addSubFolderNode(FolderNode *subFolder);
    void removeSubFolderNode(FolderNode *subFolder);

    void addNode(Node *node);
    void removeNode(Node *node);

    void addSubFolderNodes(const QList<FolderNode *> &folders);
    void removeSubFolderNodes(const QList<FolderNode *> &folders);

    void addNodes(const QList<Node *> &nodes);
    void removeNodes(const QList<Node *> &nodes);

    void removeAllSubFolderNodes();
    void removeAllNodes();

public:
    QList<FolderNode *> subFolderNodes() const;
    QList<Node *> childNodes() const;

private:
    QList<FolderNode *> m_subFolderNodes;
    QList<Node *> m_childNodes;
};

////////////////////////////////////////////////////////////////////////////////

class PROJECTEXPLORER_EXPORT VirtualFolderNode : public FolderNode
{
public:
    VirtualFolderNode(const Core::Id &id, const QString &name = QString());
    ~VirtualFolderNode();

public:
    Core::Id id() const Q_DECL_OVERRIDE;
    QString displayName() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;

public:
    void setIcon(const QIcon &icon);
    void setDisplayName(const QString &name);

private:
    Core::Id m_id;

    mutable QIcon m_icon;
    QString m_displayName;
};

////////////////////////////////////////////////////////////////////////////////

// sorting helper function

bool sortNodes(Node *n1, Node *n2);

template<class T1, class T3>
bool isSorted(const T1 &list, T3 sorter)
{
    typename T1::const_iterator it, iit, end;
    end = list.end();
    iit = list.begin();
    if (iit == end)
        return true;

    for (it = iit++; iit != end; it = iit++) {
        if (!sorter(*it, *iit))
            return false;
    }

    return true;
}


} // namespace ProjectExplorer

Q_DECLARE_METATYPE(ProjectExplorer::Node *)
Q_DECLARE_METATYPE(ProjectExplorer::FolderNode *)

#endif // BASIC_NODES_H
