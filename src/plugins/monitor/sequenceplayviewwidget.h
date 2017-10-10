#ifndef SEQUENCE_PLAY_VIEW_WIDGET_H
#define SEQUENCE_PLAY_VIEW_WIDGET_H

#include <QWidget>

namespace Monitor {
namespace Internal {
class SequencePlayViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SequencePlayViewWidget(QWidget *parent = 0);
    ~SequencePlayViewWidget();

private:
    void setBackground(QColor color);
};

} // namespace Internal
} // namespace Monitor

#endif // SEQUENCE_PLAY_VIEW_WIDGET_H
