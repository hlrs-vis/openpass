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
# \file  ComponentController.pro
# \brief This file contains the information for the QtCreator-project of the
# module ComponentController
#-----------------------------------------------------------------------------/

DEFINES += COMPONENT_CONTROLLER_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../global.pri)

SUBDIRS +=  .

INCLUDEPATH += $$SUBDIRS \
            ../../Common \
            ../../Interfaces \
            ..

SOURCES += \
        componentController.cpp \
        componentControllerImplementation.cpp \
        componentStateInformation.cpp \
        condition.cpp \
        stateManager.cpp

HEADERS += \
        condition.h \
        componentController.h \
        componentControllerGlobal.h \
        componentControllerImplementation.h \
        componentControllerCommon.h \
        componentStateInformation.h \
        stateManager.h
