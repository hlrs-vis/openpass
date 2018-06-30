#-----------------------------------------------------------------------------
# \file  OpenPassMaster.pro
# \brief This file contains the information for the QtCreator-project of the
#        OpenPass Master
#
# Copyright (c) 2017 ITK Engineering GmbH.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#-----------------------------------------------------------------------------/

include(../../../Global.pri)
QT       += core xml concurrent
QT       -= gui

win32 {
# -DLOG_TIME_ENABLED: enable logging of time information
QMAKE_CXXFLAGS += -std=c++11 -DWIN32
LIBS += -lws2_32
} else {
QMAKE_CXXFLAGS += -std=c++11 -Dunix
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenPassMaster

###############################################################################
# Use this line in Global.pri to build openPASS-Master as library
# CONFIG += USEOPENPASSMASTERASLIBRARY
###############################################################################

USEOPENPASSMASTERASLIBRARY {
    TEMPLATE = lib
    DEFINES += OPENPASSMASTERLIBRARY
} else {
    TEMPLATE = app
}

###############################################################################
# Use this line in Global.pri to use openPASS-Slave as library
# CONFIG += USEOPENPASSSLAVEASLIBRARY
###############################################################################

USEOPENPASSSLAVEASLIBRARY {
    DEFINES += USESLAVELIBRARY
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
    framework \
    importer \
    ../CoreShare \
    ../../Interfaces

INCLUDEPATH += $$SUBDIRS \

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h) \
