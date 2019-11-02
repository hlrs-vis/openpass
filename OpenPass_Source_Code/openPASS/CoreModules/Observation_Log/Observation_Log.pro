# /*********************************************************************
# * Copyright (c) 2017, 2018, 2019 in-tech GmbH
# *               2016, 2017 ITK Engineering GmbH
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

#-----------------------------------------------------------------------------
# \file  Observation_Log.pro
# \brief This file contains the information for the QtCreator-project of the
# module Observation_Log
#-----------------------------------------------------------------------------/

DEFINES += OBSERVATION_LOG_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../global.pri)

SUBDIRS +=  . \
			
INCLUDEPATH += $$SUBDIRS \
            ../../CoreFrameWork/CoreShare \
            ../../Interfaces \
            ../../Common \
            ..

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c) \
    ../../CoreFramework/CoreShare/log.cpp \
    ../../CoreFramework/CoreShare/parameters.cpp

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h) \
    ../../CoreFramework/CoreShare/log.h \
    ../../CoreFramework/CoreShare/parameters.h
