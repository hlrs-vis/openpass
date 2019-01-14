# /*********************************************************************
# * This Example Content is intended to demonstrate usage of
# * Eclipse technology. It is provided to you under the terms
# * and conditions of the Eclipse Distribution License v1.0
# * which is available at http://www.eclipse.org/org/documents/edl-v10.php
# **********************************************************************/

include(../../../Global.pri)
QT += widgets
QT -= gui

win32 {
QMAKE_CXXFLAGS += -std=c++11 -DWIN32
LIBS += -lws2_32
} else {
QMAKE_CXXFLAGS += -std=c++11 -Dunix
}

TARGET = Algorithm_Comp1
TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui

DEFINES += ALGORITHM_COMP1_LIBRARY

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
            ../../Common \
            ../../Common_PCM \
            ../../Common_Daimler \
            ../../Interfaces \

INCLUDEPATH += $$SUBDIRS \
    ..

SOURCES += \
    $$getFiles(SUBDIRS, cpp) \
    $$getFiles(SUBDIRS, cc) \
    $$getFiles(SUBDIRS, c)

HEADERS += \
    $$getFiles(SUBDIRS, hpp) \
    $$getFiles(SUBDIRS, h)
	
