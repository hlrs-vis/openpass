# /*********************************************************************
# * Copyright (c) 2017, 2018, 2019 in-tech GmbH
# * Copyright (c) 2016, 2017 ITK Engineering GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

#-----------------------------------------------------------------------------
# \file  OpenPassSlave.pro
# \brief This file contains the information for the QtCreator-project of the
#        openPASS Slave
#-----------------------------------------------------------------------------

CONFIG += OPENPASS_EXECUTABLE
include(../../../global.pri)

win32 {
# -DLOG_TIME_ENABLED: enable logging of time information
LIBS += -lws2_32
}

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui

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

SUBDIRS += \
    observationInterface \
    framework \
    importer \
    importer/road \
    modelElements \
    modelInterface \
    scheduler \
    spawnPointInterface \
    stochasticsInterface \
    worldInterface \
    eventDetectorInterface \
    manipulatorInterface \
    ../../Interfaces \
    ../../Common \
    ../CoreShare \
    ../CoreShare/cephesMIT

INCLUDEPATH += $$SUBDIRS \
    ../../Interfaces/roadInterface

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)
