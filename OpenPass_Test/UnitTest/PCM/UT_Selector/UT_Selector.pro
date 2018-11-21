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

TARGET = tst_ut_selector

INCLUDEPATH += . \
               $$MAIN_SRC_DIR/openPASS/Components_PCM/Algorithm_Selector \

HEADERS += $$MAIN_SRC_DIR/openPASS/Components_PCM/Algorithm_Selector/selector.h \
        tst_ut_selectortest.h


SOURCES += $$MAIN_SRC_DIR/openPASS/Components_PCM/Algorithm_Selector/selector.cpp \
        tst_ut_selectortest.cpp \
        main.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"
