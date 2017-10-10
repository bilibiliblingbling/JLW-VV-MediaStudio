#ifndef CORE_PANE_P_H
#define CORE_PANE_P_H

#include <QAction>

#include "pane.h"
#include "placeholder.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

template<typename T>
class PaneImpl : public Pane
{
public:
    typedef PaneImpl<T> BaseType;
    typedef PlaceHolder<T> HolderType;

public:
    explicit PaneImpl(QAction *togglePaneAction = 0, QWidget *parent = 0)
        : Pane(parent)
        , m_togglePaneAction(togglePaneAction)
    {
        if (m_togglePaneAction) {
            connect(m_togglePaneAction, &QAction::triggered,
                    this, &PaneImpl::setPaneVisible);
        }

        s_instance = this;
    }

    virtual ~PaneImpl() { s_instance = 0; }

    static PaneImpl *instance() { return s_instance; }

public:
    virtual void saveSettings(QSettings *settings) const = 0;
    virtual void restoreSettings(QSettings *settings) = 0;

public:
    virtual void placeHolderChanged(HolderType *holder)
    {
        if (m_togglePaneAction)
            m_togglePaneAction->setChecked(holder && isShown());

        updateToggleText();
    }

public:
    void setShown(bool b)
    {
        if (HolderType::current() && b) {
            QSplitter *splitter = qobject_cast<QSplitter *>(HolderType::current()->parentWidget());
            if (splitter) {
                bool disable = false;
                QSizePolicy sp = sizePolicy();
                if ((splitter->orientation() == Qt::Horizontal)
                    && (sp.horizontalPolicy() == QSizePolicy::Fixed)) {
                    disable = true;
                } else if ((splitter->orientation() == Qt::Vertical)
                           && (sp.verticalPolicy() == QSizePolicy::Fixed)) {
                    disable = true;
                }
                int idx = splitter->indexOf(HolderType::current());
                splitter->handle(idx)->setDisabled(disable);
                splitter->setStretchFactor(idx, disable? 0: 1);
            }
        }

        Pane::setShown(b);
    }

private:
    virtual void setPaneVisible(bool visible)
    {
        if (HolderType::current()) {
            if (isSuppressed() && visible) {
                setShown(true);
                setSuppressed(false);
            } else {
                setShown(visible);
            }
        }
    }

    void updateToggleText()
    {
        if (m_togglePaneAction)
            m_togglePaneAction->setEnabled(HolderType::current());
    }

private:
    QAction *m_togglePaneAction;

private:
    static PaneImpl *s_instance;
};

template<typename T>
PaneImpl<T> *PaneImpl<T>::s_instance = 0;

} // namespace Internal
} // namespace Core

#endif // CORE_PANE_P_H
