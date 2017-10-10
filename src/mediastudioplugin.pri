depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
exists($$depfile) {
    include($$depfile)
    isEmpty(MEDIASTUDIO_PLUGIN_NAME) {
        MEDIASTUDIO_PLUGIN_NAME = dummy
        warning("$$basename(depfile) does not define MEDIASTUDIO_PLUGIN_NAME.")
    }
} else {
    isEmpty(MEDIASTUDIO_PLUGIN_NAME): \
        error("MEDIASTUDIO_PLUGIN_NAME is empty. Maybe you meant to create $$basename(depfile)?")
}

TARGET = $$MEDIASTUDIO_PLUGIN_NAME

plugin_deps     = $$MEDIASTUDIO_PLUGIN_DEPENDS
plugin_recmds   = $$MEDIASTUDIO_PLUGIN_RECOMMENDS

include(../mediastudio.pri)

defineReplace(dependencyName) {
    dependencies_file =
    for(dir, MEDIASTUDIO_PLUGIN_DIRS) {
        exists($$dir/$$1/$${1}_dependencies.pri) {
            dependencies_file = $$dir/$$1/$${1}_dependencies.pri
            break()
        }
    }
    isEmpty(dependencies_file): \
        error("Plugin dependency $$dep not found")
    include($$dependencies_file)
    return($$MEDIASTUDIO_PLUGIN_NAME)
}

# for substitution in the .json
dependencyList =
for(dep, plugin_deps) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$MEDIASTUDIO_VERSION\" }"
}
for(dep, plugin_recmds) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$MEDIASTUDIO_VERSION\", \"Type\" : \"optional\" }"
}
dependencyList = $$join(dependencyList, ",$$escape_expand(\\n)")
dependencyList = "\"Dependencies\" : [$$escape_expand(\\n)$$dependencyList$$escape_expand(\\n)    ]"

# use gui precompiled header for plugins by default
isEmpty(PRECOMPILED_HEADER): PRECOMPILED_HEADER = $$PWD/shared/mediastudio_gui_pch.h

DESTDIR = $$MEDIASTUDIO_PLUGIN_PATH

LIBS += -L$$DESTDIR
INCLUDEPATH += $$OUT_PWD

# copy the plugin spec
isEmpty(TARGET) {
    error("mediastudioplugin.pri: You must provide a TARGET")
}

PLUGINJSON = $$_PRO_FILE_PWD_/$${TARGET}.json
PLUGINJSON_IN = $${PLUGINJSON}.in
exists($$PLUGINJSON_IN) {
    DISTFILES += $$PLUGINJSON_IN
    QMAKE_SUBSTITUTES += $$PLUGINJSON_IN
    PLUGINJSON = $$OUT_PWD/$${TARGET}.json
} else {
    # need to support that for external plugins
    DISTFILES += $$PLUGINJSON
}

include(rpath.pri)

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

TEMPLATE = lib
CONFIG += plugin plugin_with_soname
linux*:QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF


MIMETYPES = $$_PRO_FILE_PWD_/$${TARGET}.mimetypes.xml
exists($$MIMETYPES): DISTFILES += $$MIMETYPES

TARGET = $$qtLibraryName($$TARGET)

