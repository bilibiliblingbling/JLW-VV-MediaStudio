#ifndef VIDEO_GROUP_MANAGER_DLG_H
#define VIDEO_GROUP_MANAGER_DLG_H

#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "previewpanel.h"
#include "previewvideowidget.h"

namespace Preview {
namespace Internal {

class VideoGroupManagerDlgPrivate
{
public :
    enum {
        BtnsCount = 4,
        maxGroups = 9,
        labelsCount = 16,
        WidgetsCount = 16
    };

    QLabel *m_label;
    QComboBox *m_groupCombox[WidgetsCount];
    QSpacerItem *m_spacer;
    QDialogButtonBox *m_buttonBox;
};

class PreviewPanel;
class VideoGroupManagerDlg : public QDialog
{
    Q_OBJECT

public:
    VideoGroupManagerDlg(QWidget *parent);
    ~VideoGroupManagerDlg();

private:
    void dialogInit();
    void dialogBtnsInit();
    inline void setGroupComboxEnable();
    void loadVideosInGroupRecords();
    void accept();
    void reject();
    void yesToAllInOneGroup();
    void reset();

private:
    VideoGroupManagerDlgPrivate *d;
    QList<PreviewVideoWidget *> m_videoWidgets;
    qint8 m_videosCount;
    QVector<QColor> m_colorVector;

    //QVector<qint8> *m_videosInGroup;
    QMultiHash<quint8, PreviewVideoWidget *> *m_videoGroupsHash;
};

} // namespace Internal
} // namespace Preview

#endif // VIDEO_GROUP_MANAGER_DLG_H
