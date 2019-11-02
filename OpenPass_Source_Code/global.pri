# /*********************************************************************
# * Copyright (c) 2018 ITK Engineering GmbH
# * Copyright (c) 2018 in-tech GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

include(defaults.pri)
include(ccache.pri)

INCLUDEPATH += $$PWD/openPASS
CONFIG += c++17

## activate log time output
DEFINES += LOG_TIME_ENABLED

## simulation libraries destination sub-directory ##
SUBDIR_LIB_SIMS = "/lib"
DEFINES += SUBDIR_LIB_SIM=\\\"/lib\\\"

## gui libraries destination sub-directory ##
SUBDIR_LIB_GUIS = "/gui"
DEFINES += SUBDIR_LIB_GUI=\\\"/gui\\\"

## Qt plugins sub-directory ##
DEFINES += SUBDIR_LIB_PLUGIN=\\\"/plugin\\\"

## Export configuration ##
#CONFIG += USEOPENPASSSLAVEASLIBRARY
#CONFIG += USEOPENPASSMASTERASLIBRARY

# accumulate list of files for given directories (first parameter)
# according to file name ending (second parameter)
defineReplace(getFiles) {
    variable = $$1
    files = $$eval($$variable)
    result =
    for(file, files) {
        result += $$files($$file/*.$$2)
    }
    return($$result)
}

# prefer bundled libs in OpenPassSlave directory over system libraries
QMAKE_LFLAGS += -Wl,-rpath=\'\$\$ORIGIN\'

win32:DEFINES+=_USE_MATH_DEFINES

##################################################################
# Configuration specific for open pass libraries                 #
# Usage:                                                         #
# set "CONFIG += OPENPASS_LIBRARY" before including this file    #
##################################################################
OPENPASS_LIBRARY {
    message("[$$TARGET] Set library configuration")
    TEMPLATE = lib
    CONFIG += shared
    CONFIG -= lib_bundle
    QMAKE_LFLAGS += -L$$system_path($$EXTRA_LIB_PATH)
    QMAKE_CFLAGS += -isystem $$EXTRA_INCLUDE_PATH
    QMAKE_CXXFLAGS += -isystem $$EXTRA_INCLUDE_PATH
    Debug:DESTDIR = $${DESTDIR_SLAVE}$${SUBDIR_LIB_SIMS}
    Release:DESTDIR = $${DESTDIR_SLAVE}$${SUBDIR_LIB_SIMS}
    #debug/release predicates are buggy on linux qmake
    unix:DESTDIR=$${DESTDIR_SLAVE}$${SUBDIR_LIB_SIMS}
    message("[$$TARGET] Build path set to $$DESTDIR")
}

##################################################################
# Configuration specific for open pass executables               #
# Usage:                                                         #
# set "CONFIG += OPENPASS_EXECUTABLE" before including this file #
##################################################################
OPENPASS_EXECUTABLE {
    message("[$$TARGET] Building executable")
    QT += core xml
    QT -= gui
    TEMPLATE = app
    CONFIG -= app_bundle
    QMAKE_CFLAGS += -isystem $$EXTRA_INCLUDE_PATH
    QMAKE_CXXFLAGS += -isystem $$EXTRA_INCLUDE_PATH
    Debug:DESTDIR = $${DESTDIR_SLAVE}
    Release:DESTDIR = $${DESTDIR_SLAVE}
    #debug/release predicates are buggy on linux qmake
    unix:DESTDIR=$${DESTDIR_SLAVE}
    QMAKE_PRE_LINK += $$sprintf($$QMAKE_MKDIR_CMD, $$DESTDIR)
}
