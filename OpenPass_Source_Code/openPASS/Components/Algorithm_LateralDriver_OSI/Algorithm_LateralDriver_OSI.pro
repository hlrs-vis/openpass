#-----------------------------------------------------------------------------
# /file  AlgorithmLateralDriver.pro
# /brief This file contains the information for the QtCreator-project of the
# module AlgorithmLateralDriver
#
# Copyright (c) 2017 in-tech GmbH
#
#-----------------------------------------------------------------------------/
DEFINES += ALGORITHM_LATERALDRIVER_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../Global.pri)

SUBDIRS +=  .

INCLUDEPATH += $$SUBDIRS \
            ../../Common \
            ../../Interfaces \
            ..

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)
