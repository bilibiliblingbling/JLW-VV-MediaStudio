include(../../mediastudioplugin.pri)

QT +=
DEFINES += MONITOR_LIBRARY


HEADERS += \
    monitorplugin.h \
    monitorpanel.h \
    fileplayviewwidget.h \
    sequenceplayviewwidget.h \
    playerwidget.h \
    videocontrlwidget.h \
    videocontrlwidget_p.h


SOURCES += \
    monitorplugin.cpp \
    monitorpanel.cpp \
    fileplayviewwidget.cpp \
    sequenceplayviewwidget.cpp \
    playerwidget.cpp \
    videocontrlwidget.cpp
