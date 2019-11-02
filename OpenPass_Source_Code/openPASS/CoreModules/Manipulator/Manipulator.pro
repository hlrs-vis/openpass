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
# \file  Manipulator.pro
# \brief This file contains the information for the QtCreator-project of the
# module GenericManipulator
#-----------------------------------------------------------------------------/

DEFINES += MANIPULATOR_LIBRARY
CONFIG   += OPENPASS_LIBRARY
include (../../../global.pri)

SUBDIRS +=  . \

INCLUDEPATH += $$SUBDIRS \
            ../../Common \
            ../../CoreFrameWork/CoreShare \
            ../../Interfaces \
            ..

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c) \
    ../../Common/openScenarioDefinitions.cpp

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h) \
    ../../Common/openScenarioDefinitions.h
