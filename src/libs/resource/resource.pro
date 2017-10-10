include(../../mediastudiolibrary.pri)

QT += gui

DEFINES += MEDIASTUDIO_RESOURCE_LIBRARY
DEFINES += MEDIASTUDIO_REL_TOOLS_PATH=$$shell_quote(\"$$relative_path($$MEDIASTUDIO_LIBEXEC_PATH, $$MEDIASTUDIO_BIN_PATH)\")

unix: LIBS += $$QMAKE_LIBS_DYNLOAD

HEADERS += \
    constants.h \
    resource_global.h \
    resource.h

SOURCES += \
    resource.cpp

RESOURCES += \
    resource.qrc
