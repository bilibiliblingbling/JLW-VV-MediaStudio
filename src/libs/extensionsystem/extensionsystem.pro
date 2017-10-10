include(../../mediastudiolibrary.pri)

DEFINES += EXTENSIONSYSTEM_LIBRARY

unix: LIBS += $$QMAKE_LIBS_DYNLOAD


HEADERS += \
    extensionsystem_global.h \
    invoker.h \
    iplugin.h \
    iplugin_p.h \
    pluginmanager.h \
    pluginmanager_p.h \
    pluginspec.h \
    pluginspec_p.h \
    optionsparser.h \
    plugincollection.h

SOURCES += \
    invoker.cpp \
    iplugin.cpp \
    pluginmanager.cpp \
    pluginspec.cpp \
    optionsparser.cpp \
    plugincollection.cpp
