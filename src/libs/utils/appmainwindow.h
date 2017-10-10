#ifndef APP_MAIN_WINDOW_H
#define APP_MAIN_WINDOW_H

#include <QMainWindow>

#include "utils_global.h"

namespace Utils {


class MEDIASTUDIO_UTILS_EXPORT AppMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppMainWindow();

public slots:
    void raiseWindow();
};


} // namsepace Utils

#endif // APP_MAIN_WINDOW_H
