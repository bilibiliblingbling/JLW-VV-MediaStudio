unix: LIBS += -lUDOCA

HEADERS += \
    $$CV5_GIT_ROOT/share/AuthenticationHelper.h \
    $$CV5_GIT_ROOT/share/LocalAddressSelector.h \
    $$PWD/udocacommunicator.h \
    $$PWD/udocacontext.h \
    $$PWD/udocaobjecthelper.h

SOURCES += \
    $$CV5_GIT_ROOT/share/AuthenticationHelper.cpp \
    $$CV5_GIT_ROOT/share/LocalAddressSelector.cpp \
    $$PWD/udocacommunicator.cpp \
    $$PWD/udocacontext.cpp \
    $$PWD/udocaobjecthelper.cpp
