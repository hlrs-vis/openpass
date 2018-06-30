##################################################################
# Configuration specific for open pass testing projects          #
# Usage:                                                         #
# set "CONFIG += OPENPASS_TESTING" before including this file    #
##################################################################

OPENPASS_TESTING{
    message("[$$TARGET] Set testing configuration")
    TEMPLATE = app
    CONFIG += console
    CONFIG -= app_bundle

    INCLUDEPATH += ../../TestClasses
    LIBS += -lgtest -lgmock
}
