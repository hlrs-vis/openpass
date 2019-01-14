# /*********************************************************************
# * Copyright (c) 2017, 2018 ITK Engineering GmbH
# *
# * This program and the accompanying materials are made
# * available under the terms of the Eclipse Public License 2.0
# * which is available at https://www.eclipse.org/legal/epl-2.0/
# *
# * SPDX-License-Identifier: EPL-2.0
# **********************************************************************/

include(../../Global.pri)
TARGET = PCM
TEMPLATE = lib
CONFIG += plugin c++11 no_keywords
QT += core concurrent gui sql widgets xml

VERSION = 0.4
QMAKE_TARGET_PRODUCT = openPASS
QMAKE_TARGET_DESCRIPTION = openPASS PCM
QMAKE_TARGET_COMPANY = ITK Engineering GmbH
QMAKE_TARGET_COPYRIGHT = ITK Engineering GmbH

Release:DESTDIR = $$DIR_RELEASE
Debug:DESTDIR = $$DIR_DEBUG
win32: TARGET_EXT = .dll


include(Interfaces/Interfaces.pri)
include(Models/Models.pri)
include(Presenters/Presenters.pri)
include(Views/Views.pri)

# accumulate list of files for given directories (first parameter)
# according to file name ending (second parameter)
defineReplace(getFiles) {
    variable = $$1
    files = $$eval($$variable)
    result =
    for(file, files) {
        result += $$files($$file/*.$$2)
    }
    return($$result)
}

SUBDIRS +=  . \
            ../../openPASS/Common \
            ../../openPASS/Common_PCM \
            ../../openPASS/Common_PCM/PCM_Data \
            ../../openPASS/Common_PCM/XoscWriter \
            ../../openPASS/Common_PCM/DataStructuresBase \
            ../../openPASS/Common_PCM/DataStructuresXosc \
            ../../openPASS/Common_PCM\PCM_Importer \
            ../../openPASS/CoreFramework/CoreShare \
            ../../openPASS/Interfaces \

INCLUDEPATH += $$SUBDIRS \
               ..

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h) \

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)
