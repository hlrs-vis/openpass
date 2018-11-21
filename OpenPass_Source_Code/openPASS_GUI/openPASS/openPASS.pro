#-----------------------------------------------------------------------------
# \file  OpenPASS.pro
# \brief
#
#
# Copyright (c) 2017 Volkswagen Group of America.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
#-----------------------------------------------------------------------------/

include(../../Global.pri)
TARGET = openPASS
TEMPLATE = app
CONFIG += c++11 no_keywords
QT += core gui widgets

VERSION = 0.5
QMAKE_TARGET_PRODUCT = openPASS
QMAKE_TARGET_DESCRIPTION = openPASS
QMAKE_TARGET_COMPANY = Volkswagen Group of America
QMAKE_TARGET_COPYRIGHT = Volkswagen Group of America

Release:DESTDIR = $$DIR_RELEASE
Debug:DESTDIR = $$DIR_DEBUG

DEFINES += APPLICATION_NAME=\\\"$$TARGET\\\"
DEFINES += APPLICATION_VERSION=\\\"$$VERSION\\\"

include(Interfaces/Interfaces.pri)
include(Models/Models.pri)

RC_ICONS += \
    openPASS.ico

SOURCES += \
    openPASS.cpp
