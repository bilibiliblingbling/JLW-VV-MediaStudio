include(../../mediastudioplugin.pri)

DEFINES += KINGVISION_LIBRARY

LIBS += \
    -lStreamPlay -lStreamSDK \
    -lUDOCA

HEADERS += \
    cameraplayer.h \
    cameratree.h \
    constants.h \
    devices.h \
    devicetree.h \
    kingvisionnodes.h \
    kingvisionplugin.h \
    mediasource.h \
    mediasource_p.h \
    mediasourcefactory.h \
    playerfactory.h \
    segment.h \
    segmentplayer.h \
    segmentplayer_p.h \
    kingvisiondialog.h

SOURCES += \
    cameraplayer.cpp \
    cameratree.cpp \
    devices.cpp \
    devicetree.cpp \
    kingvisionnodes.cpp \
    kingvisionplugin.cpp \
    mediasource.cpp \
    mediasourcefactory.cpp \
    playerfactory.cpp \
    segment.cpp \
    segmentplayer.cpp \
    kingvisiondialog.cpp

include(devices/devices.pri)
