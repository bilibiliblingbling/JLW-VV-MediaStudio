#ifndef FILE_PLAY_VIEW_WIDGET_H
#define FILE_PLAY_VIEW_WIDGET_H

#include <QWidget>

namespace Monitor {
namespace Internal {

class FilePlayViewWidget : public QWidget
{
    Q_OBJECT

public:
    FilePlayViewWidget(QWidget *parent = 0);
    ~FilePlayViewWidget();

private:
    void setBackground(QColor color);

};

} // namespace Internal
} // namespace Monitor

#endif // FILE_PLAY_VIEW_WIDGET_H
