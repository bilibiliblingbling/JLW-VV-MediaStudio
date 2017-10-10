#include "appmainwindow.h"


namespace Utils {

AppMainWindow::AppMainWindow()
    : QMainWindow()
{

}

void AppMainWindow::raiseWindow()
{
    setWindowState(windowState() & ~Qt::WindowMinimized);

    raise();

    activateWindow();
}


} // namespace Utils
