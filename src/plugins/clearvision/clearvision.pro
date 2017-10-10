include(../../mediastudioplugin.pri)

DEFINES += CLEARVISION_LIBRARY

LIBS += \
    -lStreamPlay -lStreamSDK \
    -lUDOCA -lswift-1.0.0-gcc

HEADERS += \
    cameratree.h \
    constants.h \
    clearvisionnodes.h \
    clearvisionnodes_p.h \
    clearvisionplugin.h \
    devices.h \
    devices_p.h \
    mediasource.h \
    mediasource_p.h \
    mediasourcefactory.h \
    playerfactory.h \
    segment.h \
    segmentplayer.h \
    segmentplayer_p.h \
    tempwidget.h \
    controlwidget.h

SOURCES += \
    cameratree.cpp \
    clearvisionnodes.cpp \
    clearvisionplugin.cpp \
    devices.cpp \
    mediasource.cpp \
    mediasourcefactory.cpp \
    playerfactory.cpp \
    segment.cpp \
    segmentplayer.cpp \
    tempwidget.cpp \
    controlwidget.cpp


include(share.pri)
