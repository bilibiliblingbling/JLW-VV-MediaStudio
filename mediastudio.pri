!isEmpty(MEDIASTUDIO_PRI_INCLUDED): error("mediastudio.pri already included")
MEDIASTUDIO_PRI_INCLUDED = 1

MEDIASTUDIO_VERSION = 5.0.0
MEDIASTUDIO_COMPAT_VERSION = 5.0.0

linux: {
    MEDIASTUDIO_BUILD_DATE = $$system(date +\"%Y-%m-%d %H:%M:%S\")
    MEDIASTUDIO_BUILD_VERSION = $$system(git rev-parse HEAD)
}

# enable c++11
CONFIG += c++11

linux: {
    isEmpty(Platform) {
        PLATFORM = linux_$$system(arch)
    } else {
        PLATFORM = $$Platform
    }

    isEmpty(CV5_GIT_ROOT) {
        CV5_GIT_ROOT = $$system(echo $(cd $(dirname $(git rev-parse --show-toplevel)) && git rev-parse --show-toplevel))
    } else {
        CV5_GIT_ROOT = $$CV5_GIT_ROOT
    }

    CONFIG(debug, debug|release): BUILD_TYPE = debug
    CONFIG(release, release|debug): BUILD_TYPE = release

    isEmpty(CV5_SDK_ROOT) {
        CV5_SDK_ROOT = $$system(echo ${CV5_SDK_ROOT:-~/bqvision/clearvision})
    }
    BASE_DIR = $$CV5_SDK_ROOT/$$PLATFORM/$$BUILD_TYPE

    QMAKE_CXXFLAGS += \
        -isystem $$BASE_DIR/include

    INCLUDEPATH += \
        $$BASE_DIR/include \
        $$CV5_GIT_ROOT

    LIBS += \
        -L$$BASE_DIR/lib
}

defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac: RET = $$member(LIBRARY_NAME, 0)_debug
              else: win32: RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET): RET = $$LIBRARY_NAME
   return($$RET)
}


defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}


KINGVISION_COLLECTION = kingvision

isEmpty(MEDIASTUDIO_LIBRARY_BASENAME) {
    MEDIASTUDIO_LIBRARY_BASENAME = lib/$$KINGVISION_COLLECTION
}


MEDIASTUDIO_SOURCE_TREE = $$PWD
isEmpty(MEDIASTUDIO_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    MEDIASTUDIO_BUILD_TREE = $$clean_path($$OUT_PWD)
    MEDIASTUDIO_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}

MEDIASTUDIO_APP_PATH = $$MEDIASTUDIO_BUILD_TREE/bin
MEDIASTUDIO_APP_TARGET = mediastudio

MEDIASTUDIO_PREFIX = $$KINGVISION_COLLECTION/$$MEDIASTUDIO_APP_TARGET

# target output path if not set manually
isEmpty(MEDIASTUDIO_OUTPUT_PATH): MEDIASTUDIO_OUTPUT_PATH = $$MEDIASTUDIO_BUILD_TREE

MEDIASTUDIO_LIBRARY_PATH = $$MEDIASTUDIO_OUTPUT_PATH/$$MEDIASTUDIO_LIBRARY_BASENAME/$$MEDIASTUDIO_APP_TARGET
MEDIASTUDIO_PLUGIN_PATH  = $$MEDIASTUDIO_LIBRARY_PATH/plugins
MEDIASTUDIO_DATA_PATH    = $$MEDIASTUDIO_OUTPUT_PATH/share/$$KINGVISION_COLLECTION/$$MEDIASTUDIO_APP_TARGET
MEDIASTUDIO_DOC_PATH     = $$MEDIASTUDIO_OUTPUT_PATH/share/doc/$$KINGVISION_COLLECTION/$$MEDIASTUDIO_APP_TARGET
MEDIASTUDIO_BIN_PATH     = $$MEDIASTUDIO_OUTPUT_PATH/bin
MEDIASTUDIO_LIBEXEC_PATH = $$MEDIASTUDIO_OUTPUT_PATH/libexec/$$KINGVISION_COLLECTION/$$MEDIASTUDIO_APP_TARGET


# todo
#!isEqual(MEDIASTUDIO_SOURCE_TREE, $$MEDIASTUDIO_OUTPUT_PATH):copydata = 1

LINK_LIBRARY_PATH = $$MEDIASTUDIO_BUILD_TREE/$$MEDIASTUDIO_LIBRARY_BASENAME/$$MEDIASTUDIO_APP_TARGET
LINK_PLUGIN_PATH  = $$LINK_LIBRARY_PATH/plugins

INSTALL_LIBRARY_PATH = $$MEDIASTUDIO_PREFIX/$$MEDIASTUDIO_LIBRARY_BASENAME/$$MEDIASTUDIO_APP_TARGET
INSTALL_PLUGIN_PATH  = $$INSTALL_LIBRARY_PATH/plugins
INSTALL_LIBEXEC_PATH = $$MEDIASTUDIO_PREFIX/libexec/$$KINGVISION_COLLECTION/$$MEDIASTUDIO_APP_TARGET
INSTALL_DATA_PATH    = $$MEDIASTUDIO_PREFIX/share/$$KINGVISION_COLLECTION/$$MEDIASTUDIO_APP_TARGET
INSTALL_DOC_PATH     = $$MEDIASTUDIO_PREFIX/share/doc/$$KINGVISION_COLLECTION/$$MEDIASTUDIO_APP_TARGET
INSTALL_BIN_PATH     = $$MEDIASTUDIO_PREFIX/bin


INCLUDEPATH += \
    $$MEDIASTUDIO_BUILD_TREE/src \ # for <app/appversion.h>
    $$MEDIASTUDIO_SOURCE_TREE/src/libs \
    $$MEDIASTUDIO_SOURCE_TREE/tools

MEDIASTUDIO_PLUGIN_DIRS_FROM_ENVIRONMENT = $$(MEDIASTUDIO_PLUGIN_DIRS)
MEDIASTUDIO_PLUGIN_DIRS += $$split(MEDIASTUDIO_PLUGIN_DIRS_FROM_ENVIRONMENT, $$QMAKE_DIRLIST_SEP)
MEDIASTUDIO_PLUGIN_DIRS += $$MEDIASTUDIO_SOURCE_TREE/src/plugins
for(dir, MEDIASTUDIO_PLUGIN_DIRS) {
    INCLUDEPATH += $$dir
}

CONFIG += \
    depend_includepath \
    no_include_pwd

LIBS *= -L$$LINK_LIBRARY_PATH  # MediaStudio libraries
exists($$MEDIASTUDIO_LIBRARY_PATH): LIBS *= -L$$MEDIASTUDIO_LIBRARY_PATH  # library path from output path


DEFINES += MEDIASTUDIO_LIBRARY_BASENAME=\\\"$$MEDIASTUDIO_LIBRARY_BASENAME\\\"
DEFINES += MEDIASTUDIO_APP_TARGET=\\\"$$MEDIASTUDIO_APP_TARGET\\\"
DEFINES += MEDIASTUDIO_PREFIX=\\\"$$MEDIASTUDIO_PREFIX\\\"

qt {
    contains(QT, core): QT += concurrent
    contains(QT, gui): QT += widgets
}

!isEmpty(MEDIASTUDIO_PLUGIN_DEPENDS) {
    LIBS *= -L$$MEDIASTUDIO_PLUGIN_PATH  # plugin path from output directory
    LIBS *= -L$$LINK_PLUGIN_PATH  # when output path is different from MediaStudio build directory
}

# recursively resolve plugin deps
done_plugins =
for(ever) {
    isEmpty(MEDIASTUDIO_PLUGIN_DEPENDS): \
        break()

    done_plugins += $$MEDIASTUDIO_PLUGIN_DEPENDS
    for(dep, MEDIASTUDIO_PLUGIN_DEPENDS) {
        dependencies_file =
        for(dir, MEDIASTUDIO_PLUGIN_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                break()
            }
        }

        isEmpty(dependencies_file): \
            error("Plugin dependency $$dep not found")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryName($$MEDIASTUDIO_PLUGIN_NAME)
    }

    MEDIASTUDIO_PLUGIN_DEPENDS  = $$unique(MEDIASTUDIO_PLUGIN_DEPENDS)
    MEDIASTUDIO_PLUGIN_DEPENDS -= $$unique(done_plugins)
}

# recursively resolve library deps
done_libs =
for(ever) {
    isEmpty(MEDIASTUDIO_LIB_DEPENDS): \
        break()

    done_libs += $$MEDIASTUDIO_LIB_DEPENDS
    for(dep, MEDIASTUDIO_LIB_DEPENDS) {
        include($$PWD/src/libs/$$dep/$${dep}_dependencies.pri)
        LIBS += -l$$qtLibraryName($$MEDIASTUDIO_LIB_NAME)
    }

    MEDIASTUDIO_LIB_DEPENDS  = $$unique(MEDIASTUDIO_LIB_DEPENDS)
    MEDIASTUDIO_LIB_DEPENDS -= $$unique(done_libs)
}
