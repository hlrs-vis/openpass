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
# \file  LimiterAccelerationVehicleComponents.pro
# \brief This file contains the information for the QtCreator-project of the
# module LimiterAccelerationVehicleComponents
#-----------------------------------------------------------------------------/

DEFINES += LIMITER_ACCELERATION_VEHICLE_COMPONENTS_LIBRARY
CONFIG += OPENPASS_LIBRARY
include(../../../global.pri)

SUBDIRS +=  . \

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
