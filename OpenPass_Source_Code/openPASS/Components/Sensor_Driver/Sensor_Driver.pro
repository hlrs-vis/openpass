# /*********************************************************************
# * Copyright (c) 2017, 2018, 2019 in-tech GmbH
# *               2018 AMFD GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

#-----------------------------------------------------------------------------
# \file  Sensor_Driver.pro
# \brief This file contains the information for the QtCreator-project of the
# module Sensor_Driver
#-----------------------------------------------------------------------------/

DEFINES += SENSOR_DRIVER_LIBRARY
CONFIG += OPENPASS_LIBRARY

include(../../../global.pri)

SOURCES += \
    sensor_driver.cpp \
    sensor_driverCalculations.cpp \
    sensor_driverImplementation.cpp

HEADERS += \
    sensor_driver.h \
    sensor_driverCalculations.h \
    sensor_driverGlobal.h \
    sensor_driverImplementation.h \
    Signals/sensorDriverSignal.h \
    Signals/sensor_driverDefinitions.h
