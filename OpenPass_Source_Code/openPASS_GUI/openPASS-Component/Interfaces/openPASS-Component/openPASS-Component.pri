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
    $$PWD/ComponentInputItemInterface.h \
    $$PWD/ComponentInputMapInterface.h \
    $$PWD/ComponentInterface.h \
    $$PWD/ComponentItemInterface.h \
    $$PWD/ComponentMapInterface.h \
    $$PWD/ComponentOutputItemInterface.h \
    $$PWD/ComponentOutputMapInterface.h \
    $$PWD/ComponentParameterItemInterface.h \
    $$PWD/ComponentParameterMapInterface.h \
    $$PWD/ComponentScheduleInterface.h \
    $$PWD/ComponentTypedefItemInterface.h \
    $$PWD/ComponentTypedefMapInterface.h
