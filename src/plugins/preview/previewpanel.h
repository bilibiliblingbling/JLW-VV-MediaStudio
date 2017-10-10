#ifndef PREVIEW_PANEL_H
#define PREVIEW_PANEL_H

#include <QList>
#include <QWidget>
#include <QMenu>
#include <core/id.h>

#include "basicwidget.h"
#include "previewvideowidget.h"
#include "videogroupoperate.h"
#include "playerdata.h"

namespace Preview {
namespace Internal {

class PreviewVideoWidget;
class PreviewPanel : public BasicWidget
{
    Q_OBJECT

public:
    PreviewPanel(QWidget *parent = 0);
    ~PreviewPanel();

    static PreviewPanel *instance();

public:
    static Core::Id activeLayout();
    static void loadLayout(Core::Id id);

    static void updateLayoutMenu(QMenu *menu);
    static void setPreviewLayout(QAction *action);

private:
    void videoGroupContextMenu(QMenu *menu);
    void setCurrentWidgetIngroup(QAction *action);

private slots:
    void contextMenu(const QPoint &);
    void inOutEditDialog();
    void videoGroupManagerDialog();

public:
    // for video group manager dialog using
    QList<PreviewVideoWidget *> getVideoWidgets();
    qint8 getWidgetsCount() const;

private:
    void setWidgetFocus(PreviewVideoWidget * child);
    inline void setCurrentWidgetFocus(bool iscurrentWidget);
    void videoGroupOperate(PreviewVideoWidget * child, GroupOperateType t);

private slots:
    void clearCurrentWidgetTimeIn();
    void clearCurrentWidgetTimeOut();
    void clearTimeInOut();
    void playerLocateTimeIn();
    void playerLocateTimeOut();

private:
    QList<PreviewVideoWidget *> m_videoWidgets;
    PreviewVideoWidget* m_currentWidget;
    Core::Id m_activeLayout;
    //QMenu * m_groupMenu;
    QMenu *m_layoutMenu;

    PlayerData *m_playerData;
};

} // Internal namespace
} // Preview namespace

#endif // PREVIEW_PANEL_H
