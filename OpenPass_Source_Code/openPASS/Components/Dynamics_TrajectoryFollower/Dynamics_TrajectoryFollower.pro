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
# \file  Dynamics_TrajectoryFollower.pro
# \brief This file contains the information for the QtCreator-project of the
# module Dynamics_TrajectoryFollower
#-----------------------------------------------------------------------------/

DEFINES += DYNAMICS_TRAJECTORY_FOLLOWER_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../global.pri)

QT += xml

SUBDIRS +=  . \
            ../../CoreFramework/CoreShare/Cephes

INCLUDEPATH += $$SUBDIRS \
            ../../Common \
            ../../CoreFramework/CoreShare \
            ../../CoreFramework/OpenPassSlave/modelElements \
            ../../CoreFramework/OpenPassSlave/importer \
            ../../Interfaces \
            ..

SOURCES += $$getFiles(SUBDIRS, cpp) \
           $$getFiles(SUBDIRS, c) \
            ../../Common/vector2d.cpp \
            ../../CoreFramework/CoreShare/log.cpp \
            ../../CoreFramework/CoreShare/xmlParser.cpp \
           ../../CoreFramework/OpenPassSlave/importer/csvParser.cpp \
           ../../CoreFramework/OpenPassSlave/importer/trajectory.cpp \
           ../../CoreFramework/OpenPassSlave/importer/trajectoryImporter.cpp

HEADERS += $$getFiles(SUBDIRS, h) \
            ../../Common/vector2d.h \
            ../../CoreFramework/CoreShare/log.h \
            ../../CoreFramework/CoreShare/xmlParser.h \
           ../../CoreFramework/OpenPassSlave/importer/csvParser.h \
           ../../CoreFramework/OpenPassSlave/importer/trajectory.h \
           ../../CoreFramework/OpenPassSlave/importer/trajectoryImporter.h

