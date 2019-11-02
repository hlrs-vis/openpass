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
# \file  AlgorithmAgentFollowingDriverModel.pro
# \brief This file contains the information for the QtCreator-project of the
# module AlgorithmAgentFollowingDriverModel
#-----------------------------------------------------------------------------/

DEFINES += ALGORITHM_AGENTFOLLOWINGDRIVERMODEL_LIBRARY
CONFIG += OPENPASS_LIBRARY

include(../../../global.pri)

SOURCES += \
    AlgorithmAgentFollowingDriverModel.cpp \
    AlgorithmAgentFollowingDriverModelImplementation.cpp

HEADERS += \
    AlgorithmAgentFollowingDriverModel.h \
    AlgorithmAgentFollowingDriverModelGlobal.h \
    AlgorithmAgentFollowingDriverModelImplementation.h
