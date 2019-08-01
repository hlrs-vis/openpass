# /*********************************************************************
# * Copyright (c) 2017, 2018, 2019 in-tech GmbH
# * Copyright (c) 2016, 2017 ITK Engineering GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

#-----------------------------------------------------------------------------
# \file  OpenPassMaster.pro
# \brief This file contains the information for the QtCreator-project of the
#        openPASS Master
#-----------------------------------------------------------------------------

CONFIG += OPENPASS_EXECUTABLE
include(../../../global.pri)

QT += concurrent widgets xml

SRC_CORESHARE = ../CoreShare/xmlParser.cpp \
                ../CoreShare/log.cpp

INC_CORESHARE = ../CoreShare/xmlParser.h \
                ../CoreShare/log.h

INCLUDEPATH += \
    framework \
    importer \
    ../CoreShare \
    ../../Interfaces \
    ../../Common

SOURCES += \
    $$SRC_CORESHARE \
    framework/main.cpp \
    framework/processManager.cpp \
    importer/masterConfigImporter.cpp

HEADERS += \
    $$INC_CORESHARE \
    openPassMaster.h \
    framework/processManager.h \
    importer/masterConfigImporter.h \
    framework/masterConfig.h \
    framework/slaveConfig.h
