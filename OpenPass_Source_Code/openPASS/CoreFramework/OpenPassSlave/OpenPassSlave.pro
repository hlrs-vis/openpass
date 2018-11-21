# /*********************************************************************
# * Copyright (c) 2017 ITK Engineering GmbH
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
#        OpenPass Slave
#-----------------------------------------------------------------------------/

include(../../../Global.pri)
QT       += core xml
QT       -= gui

win32 {
# -DLOG_TIME_ENABLED: enable logging of time information
QMAKE_CXXFLAGS += -std=c++11 -DWIN32
LIBS += -lws2_32
} else {
QMAKE_CXXFLAGS += -std=c++11 -Dunix
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenPassSlave

###############################################################################
# Use this line in Global.pri to build openPASS-Slave as library
# CONFIG += USEOPENPASSSLAVEASLIBRARY
###############################################################################

USEOPENPASSSLAVEASLIBRARY {
    TEMPLATE = lib
    DEFINES += OPENPASSSLAVELIBRARY
} else {
    TEMPLATE = app
}

Release:DESTDIR = $$DIR_RELEASE
Debug:DESTDIR = $$DIR_DEBUG

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
    collisionDetectionInterface \
    worldInterface \
    ../CoreShare \
    ../CoreShare/cephesMIT \
    ../../Common \
    ../../Interfaces \
    ../../Interfaces/roadInterface

INCLUDEPATH += $$SUBDIRS

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)
