TEMPLATE = subdirs
SUBDIRS = \
    openPASS/OpenPass_SIM.pro \
    openPASS_GUI/openPASS_GUI.pro


# copy resources
include(Global.pri)

CONFIG(debug, debug|release) {
DIR_BIN = $$DIR_DEBUG
} else {
DIR_BIN = $$DIR_RELEASE
}
DIR_PRO = $${PWD}

win32 {
DIR_PRO ~= s,/,\\,g
DIR_BIN ~= s,/,\\,g
copydata.commands = xcopy $$DIR_PRO\openPASS_Resource\OpenPass_PCM_UseCase $$DIR_BIN /S /D
} else {
copydata.commands = cp -ru $$DIR_PRO/openPASS_Resource/OpenPass_PCM_UseCase $$DIR_BIN
}
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
