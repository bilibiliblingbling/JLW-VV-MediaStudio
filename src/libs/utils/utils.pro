include(../../mediastudiolibrary.pri)

QT += gui

DEFINES += MEDIASTUDIO_UTILS_LIBRARY
DEFINES += MEDIASTUDIO_REL_TOOLS_PATH=$$shell_quote(\"$$relative_path($$MEDIASTUDIO_LIBEXEC_PATH, $$MEDIASTUDIO_BIN_PATH)\")

unix: LIBS += $$QMAKE_LIBS_DYNLOAD

HEADERS += \
    algorithm.h \
    appmainwindow.h \
    fancymainwindow.h \
    fadingindicator.h \
    fadingindicator_p.h \
    fileutils.h \
    hostosinfo.h \
    ipvalidator.h \
    itemviews.h \
    mimedatabase.h \
    navigationtreeview.h \
    osspecificaspects.h \
    proxyaction.h \
    qtcassert.h \
    savefile.h \
    skinbutton.h \
    stringutils.h \
    styledbar.h \
    stylehelper.h \
    tmplog.h \
    utils_global.h \
    wizard.h


SOURCES += \
    appmainwindow.cpp \
    fancymainwindow.cpp \
    fadingindicator.cpp \
    fileutils.cpp \
    hostosinfo.cpp \
    ipvalidator.cpp \
    itemviews.cpp \
    mimedatabase.cpp \
    navigationtreeview.cpp \
    proxyaction.cpp \
    qtcassert.cpp \
    savefile.cpp \
    skinbutton.cpp \
    stringutils.cpp \
    styledbar.cpp \
    stylehelper.cpp \
    wizard.cpp


include(communicator/communicator.pri)
include(theme/theme.pri)
include(tooltip/tooltip.pri)
