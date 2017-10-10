include(../../mediastudioplugin.pri)

QT +=
DEFINES += TIMELINE_LIBRARY

LIBS += \
    -lqwt-qt5

################################################################################

HEADERS += \
    constants.h \
    sequencecanvas.h \
    sequencetimepicker.h \
    sequencetreewidget.h \
    sequencewidget.h \
    timeline_global.h \
    timelineplugin.h

SOURCES += \
    sequencecanvas.cpp \
    sequencetimepicker.cpp \
    sequencetreewidget.cpp \
    sequencewidget.cpp \
    timelineplugin.cpp

