include(../../mediastudioplugin.pri)

DEFINES += PLAYER_LIBRARY

LIBS += \
    -lStreamPlay -lStreamSDK \
    -lswift-1.0.0-gcc


HEADERS += \
    basicscreen.h \
    basicscreen_p.h \
    constants.h \
    iplayer.h \
    iplayerfactory.h \
    iscreen.h \
    mediafile.h \
    player_global.h \
    playerplugin.h \
    streamhelper.h


SOURCES += \
    basicscreen.cpp \
    iplayer.cpp \
    iplayerfactory.cpp \
    iscreen.cpp \
    mediafile.cpp \
    playerplugin.cpp \
    streamhelper.cpp

include(share.pri)
