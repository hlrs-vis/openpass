## executable destination directories ##
win32 {
    DIR_DEBUG = "C:\OpenPASS\BinDebug"
    DIR_RELEASE = "C:\OpenPASS\BinRelease"
}

unix {
    DIR_DEBUG = "/OpenPASS/BinDebug"
    DIR_RELEASE = "/OpenPASS/BinRelease"
}

Debug:DESTDIR=$$DIR_DEBUG
Release:DESTDIR=$$DIR_RELEASE

#debug/release predicates are buggy on linux qmake
unix:DESTDIR=$$DIR_DEBUG
#unix:DESTDIR=$$DIR_RELEASE

# adjust path to your needs
win32 {
    EXTRA_INCLUDE_PATH=C:/OpenPass/ThirdParty/include
    EXTRA_LIB_PATH=C:/OpenPass/ThirdParty/lib
}

unix {
    EXTRA_INCLUDE_PATH=/OpenPASS/ThirdParty/include
    EXTRA_LIB_PATH=/OpenPASS/ThirdParty/lib
}

## simulation libraries destination sub-directory ##
SUBDIR_LIB_SIMS = "/sim"
DEFINES += SUBDIR_LIB_SIM=\\\"/sim\\\"

## gui libraries destination sub-directory ##
SUBDIR_LIB_GUIS = "/gui"
DEFINES += SUBDIR_LIB_GUI=\\\"/gui\\\"

## Qt plugins sub-directory ##
DEFINES += SUBDIR_LIB_PLUGIN=\\\"/plugin\\\"

## Export configuration ##
#CONFIG += USEOPENPASSSLAVEASLIBRARY
#CONFIG += USEOPENPASSMASTERASLIBRARY

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

# prefer bundled libs in OpenPassSlave directory over system libraries
QMAKE_LFLAGS += -Wl,-rpath=\'\$\$ORIGIN\'

win32:DEFINES+=_USE_MATH_DEFINES

##################################################################
# Configuration specific for open pass libraries                 #
# Usage:                                                         #
# set "CONFIG += OPENPASS_LIBRARY" before including this file    #
##################################################################
OPENPASS_LIBRARY {
    message("[$$TARGET] Set library configuration")
    TEMPLATE = lib
    CONFIG += shared
    CONFIG -= lib_bundle
    QMAKE_LFLAGS += -L$$system_path($$EXTRA_LIB_PATH)
    INCLUDEPATH += $$EXTRA_INCLUDE_PATH
    message("[$$TARGET] Build path set to $$DESTDIR")
}
