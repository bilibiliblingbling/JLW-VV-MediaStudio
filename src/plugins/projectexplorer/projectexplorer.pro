include(../../mediastudioplugin.pri)

QT +=

DEFINES += PROJECTEXPLORER_LIBRARY


HEADERS += \
    basicmodel.h \
    basicnodes.h \
    basictree.h \
    basictreemanager.h \
    basictreeview.h \
    basictreewidget.h \
    camera.h \
    constants.h \
    corelistener.h \
    fastpreview.h \
    iprojectmanager.h \
    iqueryroutelistcallback.h \
    mediasource.h \
    mediasourcetimer.h \
    metadata.h \
    metamimedata.h \
    monitor.h \
    project.h \
    projectnode.h \
    projectexplorer_global.h \
    projectexplorerplugin.h \
    segment.h \
    segrepotreewidget.h \
    selecttreedialog.h \
    selecttreedialog_p.h \
    seqseg.h \
    sequence.h \
    sessionmanager.h

SOURCES += \
    basicmodel.cpp \
    basicnodes.cpp \
    basictree.cpp \
    basictreemanager.cpp \
    basictreeview.cpp \
    basictreewidget.cpp \
    camera.cpp \
    corelistener.cpp \
    fastpreview.cpp \
    iprojectmanager.cpp \
    iqueryroutelistcallback.cpp \
    mediasource.cpp \
    mediasourcetimer.cpp \
    metadata.cpp \
    metamimedata.cpp \
    monitor.cpp \
    project.cpp \
    projectnode.cpp \
    projectexplorerplugin.cpp \
    segment.cpp \
    segrepotreewidget.cpp \
    selecttreedialog.cpp \
    seqseg.cpp \
    sequence.cpp \
    sessionmanager.cpp
