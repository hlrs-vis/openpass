# /*********************************************************************
# * Copyright (c) 2017, 2018, 2019 in-tech GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

#-----------------------------------------------------------------------------
# \file  Sensor_OSI.pro
# \brief This file contains the information for the QtCreator-project of the
# module Sensor_OSI
#-----------------------------------------------------------------------------/

DEFINES += SENSOR_OBJECT_DETECTOR_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../global.pri)

#for OSI config parsing
QT += xml

SUBDIRS += .

INCLUDEPATH += $$SUBDIRS \
    ../../Common \
    ../../CoreFramework/CoreShare \
    ../../Interfaces \
    ../../Interfaces/RoadInterface \
    ../../CoreModules/World_OSI

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c) \

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)

win32 {
LIBS += \
    -lopen_simulation_interface \
    -lzmq \
    -lprotobuf
} else {
LIBS += \
    -lopen_simulation_interface \
    -lzmq \
    -lprotobuf
}
