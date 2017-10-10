include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))

isEmpty(MEDIASTUDIO_LIB_NAME) {
    warning("$$replace(_PRO_FILE_PWD_, ([^/]+$), \\1) doesn't provide a MEDIASTUDIO_LIB_NAME!")
    MEDIASTUDIO_LIB_NAME = dummy
}
TARGET = $$MEDIASTUDIO_LIB_NAME

include(../mediastudio.pri)

# use precompiled header for libraries by default
isEmpty(PRECOMPILED_HEADER): PRECOMPILED_HEADER = $$PWD/shared/mediastudio_pch.h


TARGET = $$qtLibraryName($$TARGET)
DESTDIR = $$MEDIASTUDIO_LIBRARY_PATH

include(rpath.pri)


TEMPLATE = lib
CONFIG  += shared dll

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols
