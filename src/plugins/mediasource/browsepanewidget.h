#ifndef MEDIASOURCE_BROWSE_PANE_WIDGET_H
#define MEDIASOURCE_BROWSE_PANE_WIDGET_H

#include <QWidget>

#include <core/id.h>

QT_BEGIN_NAMESPACE
class QComboBox;
class QToolButton;
class QStackedWidget;
QT_END_NAMESPACE

namespace Core {
class Command;
class IContext;
}

namespace ProjectExplorer {
class MediaSource;
}

namespace Utils {
class StyledBar;
}


namespace MediaSource {
namespace Internal {

class TreeWidget;
class QueryWidget;
class BrowsePaneWidget : public QWidget
{
    Q_OBJECT

public:
    BrowsePaneWidget();
    ~BrowsePaneWidget();

    static BrowsePaneWidget *instance();

public:
    static void setCurrentMediaSource(ProjectExplorer::MediaSource *currentMediaSource);
    static void browseMediaSource(ProjectExplorer::MediaSource *mediasource);

public:
    Core::Command *command(const QString &title) const;

private slots:
    void activateSubWidget();
    void closeBrowsePane();

private:
    Core::IContext *m_context;
    Utils::StyledBar *m_toolbar;
    QComboBox *m_comboBox;
    QToolButton *m_closeButton;
    QStackedWidget *m_stackedWidget;

    TreeWidget *m_deviceWidget;
    QueryWidget *m_queryWidget;

private:
    QList<QAction *> m_actions;
    QList<Core::Command *> m_commands;
};

} // namespace Internal
} // namespace MediaSource

#endif // MEDIASOURCE_BROWSE_PANE_WIDGET_H
