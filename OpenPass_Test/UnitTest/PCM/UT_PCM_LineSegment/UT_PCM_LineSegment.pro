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

Release:DESTDIR = $$DIR_RELEASE
Debug:DESTDIR = $$DIR_DEBUG

TARGET = tst_ut_pcm_linesegmenttest

INCLUDEPATH += . \
               $$MAIN_SRC_DIR/openPASS/Common_PCM/PCM_Data \
               $$MAIN_SRC_DIR/openPASS/Common

HEADERS += $$MAIN_SRC_DIR/openPASS/Common_PCM/PCM_Data/pcm_lineSegment.h \
        $$MAIN_SRC_DIR/openPASS/Common_PCM/PCM_Data/pcm_point.h \
        $$MAIN_SRC_DIR/openPASS/Common/Vector2d.h \
        $$MAIN_SRC_DIR/openPASS/Common/CommonTools.h \
        tst_ut_pcm_linesegmenttest.h


SOURCES += $$MAIN_SRC_DIR/openPASS/Common_PCM/PCM_Data/pcm_lineSegment.cpp\
        $$MAIN_SRC_DIR/openPASS/Common_PCM/PCM_Data/pcm_point.cpp \
        $$MAIN_SRC_DIR/openPASS/Common/Vector2d.cpp \
        tst_ut_pcm_linesegmenttest.cpp \
        main.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"
