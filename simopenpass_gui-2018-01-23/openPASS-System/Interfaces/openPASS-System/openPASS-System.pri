#----------------------------------------------------------------------------- 
# Copyright (c) 2017 Volkswagen Group of America. 
# 
# This program and the accompanying materials are made 
# available under the terms of the Eclipse Public License 2.0 
# which is available at https://www.eclipse.org/legal/epl-2.0/ 
# 
# SPDX-License-Identifier: EPL-2.0 
#-----------------------------------------------------------------------------/ 

!contains(INCLUDEPATH, $$clean_path($$PWD/..)) {
    INCLUDEPATH += $$clean_path($$PWD/..)
}

HEADERS += \
    $$PWD/SystemComponentInputItemInterface.h \
    $$PWD/SystemComponentInputMapInterface.h \
    $$PWD/SystemComponentItemInterface.h \
    $$PWD/SystemComponentManagerInterface.h \
    $$PWD/SystemComponentMapInterface.h \
    $$PWD/SystemComponentOutputItemInterface.h \
    $$PWD/SystemComponentOutputMapInterface.h \
    $$PWD/SystemComponentParameterItemInterface.h \
    $$PWD/SystemComponentParameterMapInterface.h \
    $$PWD/SystemComponentScheduleInterface.h \
    $$PWD/SystemConnectionMapInterface.h \
    $$PWD/SystemConnectionItemInterface.h \
    $$PWD/SystemInterface.h \
    $$PWD/SystemItemInterface.h \
    $$PWD/SystemMapInterface.h
