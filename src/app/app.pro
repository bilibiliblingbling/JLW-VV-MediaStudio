include(../../mediastudio.pri)
include(../shared/qtsingleapplication/qtsingleapplication.pri)

TEMPLATE = app

TARGET   = $$MEDIASTUDIO_APP_TARGET
DESTDIR  = $$MEDIASTUDIO_APP_PATH
VERSION  = $$MEDIASTUDIO_VERSION


include(../rpath.pri)
LIBS *= \
    -l$$qtLibraryName(ExtensionSystem) \
    -l$$qtLibraryName(Aggregation)


HEADERS +=


SOURCES += \
    main.cpp


DISTFILES += \
    $$PWD/app_version.h.in


QMAKE_SUBSTITUTES += \
    $$PWD/app_version.h.in
