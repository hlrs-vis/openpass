#-----------------------------------------------------------------------------
# \file  World.pro
# \brief This file contains the information for the QtCreator-project of the
#        world module
#
# Copyright (c) 2018 in-tech GmbH
#
#-----------------------------------------------------------------------------/
DEFINES += WORLD_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../Global.pri)

SUBDIRS += \
    . \
    Localization \
    OWL \
    ../../Common \
    ../../Interfaces

INCLUDEPATH += \
    $$SUBDIRS \
    ../../Interfaces \
    ../../Interfaces/roadInterface \
    ..

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)

LIBS += -lopen_simulation_interface -lprotobuf
