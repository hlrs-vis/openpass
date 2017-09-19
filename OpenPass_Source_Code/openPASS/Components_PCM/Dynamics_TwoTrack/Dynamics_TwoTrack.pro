#-----------------------------------------------------------------------------
# \file  Dynamics_TwoTrack.pro
# \brief This file contains the information for the QtCreator-project of the
#        module Dynamics_TwoTrack
#
# Copyright (c) 2017 ITK Engineering GmbH.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#-----------------------------------------------------------------------------/

include(../../../Global.pri)
QT -= gui

win32 {
QMAKE_CXXFLAGS += -std=c++11 -DWIN32
LIBS += -lws2_32
} else {
QMAKE_CXXFLAGS += -std=c++11 -Dunix
}

TARGET = Dynamics_TwoTrack
TEMPLATE = lib

Release:DESTDIR = $$DIR_RELEASE
Debug:DESTDIR = $$DIR_DEBUG

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui

DEFINES += DYNAMICS_TWOTRACK_LIBRARY

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

SUBDIRS +=  . \
            ../../Common \
            ../../Common_PCM \
            ../../Interfaces

INCLUDEPATH += $$SUBDIRS

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)
