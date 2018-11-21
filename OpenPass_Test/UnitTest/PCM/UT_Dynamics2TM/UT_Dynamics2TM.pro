# /*********************************************************************
# * Copyright (c) 2018 ITK Engineering GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

include(../../UT.pri)

TARGET = tst_ut_dynamics2tmtest

INCLUDEPATH += . \
               $$MAIN_SRC_DIR/openPASS/Components_PCM/Dynamics_TwoTrack \
               $$MAIN_SRC_DIR/openPASS/Common \

HEADERS += \
    $$MAIN_SRC_DIR/openPASS/Components_PCM/Dynamics_TwoTrack/dynamics_twotrack_vehicle.h \
    $$MAIN_SRC_DIR/openPASS/Components_PCM/Dynamics_TwoTrack/dynamics_twotrack_tire.h \
    $$MAIN_SRC_DIR/openPASS/Components_PCM/Dynamics_TwoTrack/dynamics_twotrack_local.h \
    $$MAIN_SRC_DIR/openPASS/Common/vector2d.h \
    tst_ut_dynamics2tmtest.h \

SOURCES += \
    $$MAIN_SRC_DIR/openPASS/Components_PCM/Dynamics_TwoTrack/dynamics_twotrack_vehicle.cpp \
    $$MAIN_SRC_DIR/openPASS/Components_PCM/Dynamics_TwoTrack/dynamics_twotrack_tire.cpp \
    $$MAIN_SRC_DIR/openPASS/Components_PCM/Dynamics_TwoTrack/dynamics_twotrack_local.cpp \
    $$MAIN_SRC_DIR/openPASS/Common/vector2d.cpp \
    tst_ut_dynamics2tmtest.cpp \
    main.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"
