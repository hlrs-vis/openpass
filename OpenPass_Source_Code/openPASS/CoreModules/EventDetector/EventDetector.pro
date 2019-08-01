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
# \file  EventDetector.pro
# \brief This file contains the information for the QtCreator-project of the
# module EventDetector
#-----------------------------------------------------------------------------/

DEFINES += EVENT_DETECTOR_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../global.pri)

SUBDIRS +=  . \
            ./Conditions

INCLUDEPATH += $$SUBDIRS \
            ../../Interfaces \
            ../../CoreFramework/CoreShare \
            ../../Common \
            ..

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c) \
    ../../CoreFramework/CoreShare/log.cpp \
    ../../Common/vector2d.cpp

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h) \
    ../../CoreFramework/CoreShare/log.h \
    ../../Common/vector2d.h


