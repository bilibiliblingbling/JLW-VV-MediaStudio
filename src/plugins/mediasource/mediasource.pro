include(../../mediastudioplugin.pri)

DEFINES += MEDIASOURCE_LIBRARY


HEADERS += \
    browsepanewidget.h \
    cameralistedit.h \
    cameralistedit_p.h \
    cameranode.h \
    cameratree.h \
    constants.h \
    devicetree.h \
    imediasourcefactory.h \
    iquerytimerangefactory.h \
    mediasource_global.h \
    mediasourcenode.h \
    mediasourceplugin.h \
    mediasourcetree.h \
    mediasourcetreewidget.h \
    querywidget.h \
    querywidget_p.h \
    segmentnode.h \
    segmenttree.h \
    treewidget.h \
    mediasourcedialog.h

SOURCES += \
    browsepanewidget.cpp \
    cameralistedit.cpp \
    cameranode.cpp \
    cameratree.cpp \
    devicetree.cpp \
    imediasourcefactory.cpp \
    iquerytimerangefactory.cpp \
    mediasourcenode.cpp \
    mediasourceplugin.cpp \
    mediasourcetree.cpp \
    mediasourcetreewidget.cpp \
    querywidget.cpp \
    segmentnode.cpp \
    segmenttree.cpp \
    treewidget.cpp \
    mediasourcedialog.cpp


RESOURCES += \
    mediasource.qrc
