include(../../mediastudioplugin.pri)

DEFINES += CORE_LIBRARY
QT += printsupport


HEADERS += \
    browsepane.h \
    commandcombobox.h \
    constants.h \
    core_global.h \
    coreplugin.h \
    editmode.h \
    fancyactionbar.h \
    fancytabwidget.h \
    icontext.h \
    icore.h \
    icorelistener.h \
    id.h \
    imode.h \
    inavigationwidgetfactory.h \
    iwelcomepage.h \
    iwizardfactory.h \
    mainwindow.h \
    manhattanstyle.h \
    minisplitter.h \
    modemanager.h \
    monitorpane.h \
    navigationsubwidget.h \
    navigationwidget.h \
    pane.h \
    pane_p.h \
    placeholder.h \
    previewpane.h \
    statusbarmanager.h \
    statusbarwidget.h \
    styleanimator.h \
    timelinepane.h \
    windowsupport.h



SOURCES +=  \
    browsepane.cpp \
    commandcombobox.cpp \
    coreplugin.cpp \
    editmode.cpp \
    fancyactionbar.cpp \
    fancytabwidget.cpp \
    icontext.cpp \
    icore.cpp \
    icorelistener.cpp \
    id.cpp \
    imode.cpp \
    inavigationwidgetfactory.cpp \
    iwelcomepage.cpp \
    mainwindow.cpp \
    manhattanstyle.cpp \
    minisplitter.cpp \
    monitorpane.cpp \
    modemanager.cpp \
    navigationsubwidget.cpp \
    navigationwidget.cpp \
    pane.cpp \
    placeholder.cpp \
    previewpane.cpp \
    statusbarmanager.cpp \
    statusbarwidget.cpp \
    styleanimator.cpp \
    timelinepane.cpp \
    windowsupport.cpp



RESOURCES += \
    core.qrc

include(action/action.pri)
