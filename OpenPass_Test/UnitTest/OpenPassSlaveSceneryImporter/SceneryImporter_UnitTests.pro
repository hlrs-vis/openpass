#-----------------------------------------------------------------------------
# \file  OpenPassSlave_UnitTests.pro
# \brief This file contains tests for the OpenPassSlave module
#
# Copyright (c) 2018 in-tech GmbH
# on behalf of BMW AG
#-----------------------------------------------------------------------------/
QT += widgets xml

CONFIG += OPENPASS_TESTING
include(../../Testing.pri)

INCLUDEPATH += \
            ../../../OpenPass_Source_Code/openPASS/Common \
            ../../../OpenPass_Source_Code/openPASS/CoreFramework/CoreShare \
            ../../../OpenPass_Source_Code/openPASS/CoreFramework/CoreShare/cephesMIT \
            ../../../OpenPass_Source_Code/openPASS/CoreFramework/OpenPassSlave/framework \
            ../../../OpenPass_Source_Code/openPASS/CoreFramework/OpenPassSlave/importer \
            ../../../OpenPass_Source_Code/openPASS/Interfaces \
            ../../../OpenPass_Source_Code/openPASS/Interfaces/roadInterface

SOURCES += \
    ../../../OpenPass_Source_Code/openPASS/Common/vector2d.cpp \
    ../../../OpenPass_Source_Code/openPASS/Common/utilities.cpp \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/CoreShare/cephesMIT/fresnl.c \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/CoreShare/cephesMIT/polevl.c \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/CoreShare/cephesMIT/const.c \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/CoreShare/log.cpp \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/CoreShare/xmlParser.cpp \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/OpenPassSlave/importer/road.cpp \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/OpenPassSlave/importer/road/roadSignal.cpp \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/OpenPassSlave/importer/road/roadObject.cpp \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/OpenPassSlave/importer/scenery.cpp \
    ../../../OpenPass_Source_Code/openPASS/CoreFramework/OpenPassSlave/importer/sceneryImporter.cpp \
    SceneryImporter_UnitTests.cpp
