# /*********************************************************************
# * Copyright (c) 2017, 2018 ITK Engineering GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

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
