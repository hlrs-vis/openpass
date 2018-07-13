#-----------------------------------------------------------------------------
# /file  AlgorithmLongitudinalDriver.pro
# /brief This file contains the information for the QtCreator-project of the
# module AlgorithmLongitudinalDriver
#
# Copyright (c) 2017 AMFD GmbH
#
#-----------------------------------------------------------------------------/
DEFINES += ALGORITHM_LONGITUDINAL_DRIVER_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../Global.pri)

SUBDIRS +=  . \

INCLUDEPATH += $$SUBDIRS \
            ../../Common \
            ../../Common_BMW/Signals \
            ../../Interfaces \
            ..

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)
