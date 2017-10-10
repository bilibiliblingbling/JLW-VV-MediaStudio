#ifndef MINI_SPLITTER_H
#define MINI_SPLITTER_H

#include <QSplitter>

#include "core_global.h"


QT_BEGIN_NAMESPACE
class QSplitterHandle;
QT_END_NAMESPACE

namespace Core {

/*! This is a simple helper-class to obtain mac-style 1-pixel wide splitters */
class CORE_EXPORT MiniSplitter : public QSplitter
{
public:
    enum SplitterStyle { Dark, Light };

    MiniSplitter(QWidget *parent = 0, SplitterStyle style = Dark);
    MiniSplitter(Qt::Orientation orientation, SplitterStyle style = Dark);

protected:
    QSplitterHandle *createHandle();

private:
    SplitterStyle m_style;
};

} // namespace Core

#endif // MINI_SPLITTER_H
