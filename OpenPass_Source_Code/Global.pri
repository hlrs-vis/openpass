## executable destination directories ##
DIR_DEBUG = "C:\OpenPASS\BinDebug"
DIR_RELEASE = "C:\OpenPASS\BinRelease"

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
