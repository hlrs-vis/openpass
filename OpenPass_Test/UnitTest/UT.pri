# /*********************************************************************
# * Copyright (c) 2018 ITK Engineering GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

MAIN_SRC_DIR = $$PWD/../../OpenPass_Source_Code
include($$MAIN_SRC_DIR/Global.pri)

QT       += testlib
QT       -= gui

win32 {
QMAKE_CXXFLAGS += -std=c++11 -DWIN32
LIBS += -lws2_32
} else {
QMAKE_CXXFLAGS += -std=c++11 -Dunix
}

Release:DESTDIR = $$DIR_RELEASE
Debug:DESTDIR = $$DIR_DEBUG

CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
