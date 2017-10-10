#ifndef CORE_PANE_H
#define CORE_PANE_H

#include <QWidget>

#include "core_global.h"

namespace Core {

namespace Internal { class PanePrivate; }

class CORE_EXPORT Pane : public QWidget
{
    Q_OBJECT

public:
    explicit Pane(QWidget *parent = 0);
    virtual ~Pane();

public:
    void setWidget(QWidget *widget);
    bool validateWidget() const;

    qint32 storedSize() const;
    void restoreSize(qint32 size);

public:
    virtual bool isShown() const;
    virtual void setShown(bool b);

    virtual bool isSuppressed() const;
    virtual void setSuppressed(bool b);

public:
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

public:
    QSize sizeHint() const;

protected:
    void resizeEvent(QResizeEvent *re);

private:
    void clearWidget();

private:
    Internal::PanePrivate *d;
};

} // namespace

#endif // CORE_PANE_H
