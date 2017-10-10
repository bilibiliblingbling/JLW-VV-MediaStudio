#ifndef IN_OUT_EDIT_DIAGLOG_H
#define IN_OUT_EDIT_DIAGLOG_H

#include <QDialog>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include "playerdata.h"
#include "previewvideowidget.h"

namespace Preview {
namespace Internal {

class InOutEditDialogPrivate
{
public:
    QLabel *m_timeSpanStaticLabel;
    QLabel *m_timeFromLabel;
    QLabel *m_timeToStaticLabel;
    QLabel *m_timeToLabel;
    QLabel *m_timeLengthStaticLabel;
    QLabel *m_timeLengthLabel;
    QRadioButton *m_absoluteTimeRadioBtn;
    QRadioButton *m_relativeTimeRadioBtn;
    QCheckBox *m_setInTimeCheckBox;
    QCheckBox *m_setOutTimeCheckBox;
    QDateTimeEdit *m_setTimeInEdit;
    QDateTimeEdit *m_setTimeOutEdit;
    QDialogButtonBox *m_dlgBtnBox;
};


class InOutEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InOutEditDialog(QWidget *parent, PlayerData *playerData, PreviewVideoWidget *currentWidget);
    //InOutEditDialog(QWidget *parent, const PlayerDataPtr &playerData);
    ~InOutEditDialog();

private:
    void memberwidgetsInit();
    void dialogButtonBoxInit();
    void setTime();

private slots:
    void onAbsoluteTimeRadionChecked();
    void onRelativeTimeRadioChecked();
    void onSetInTimeCheckBoxChecked(bool);
    void onSetOutTimeCheckBoxChecked(bool);

private:
    void setTimeInEditDisPlayAbsoluteTime();
    void setTimeInEditDisPlayRelativeTime();

    void setTimeOutEditDisPlayAbsoluteTime();
    void setTimeOutEditDisPlayRelativeTime();
    void accept();
    void reject();

private:
    InOutEditDialogPrivate *d;
    PlayerData *m_playerData;
    PlayBackData *m_playBackData;
    LivePlayerData *m_livePlayData;
    PreviewVideoWidget *m_currentWidget;
};

} // namespace Internal
} // namespace Preview

#endif // IN_OUT_EDIT_DIAGLOG_H
