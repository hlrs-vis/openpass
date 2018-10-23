!contains(INCLUDEPATH, $$clean_path($$PWD/..)) {
    INCLUDEPATH += $$clean_path($$PWD/..)
}

HEADERS += \
    $$PWD/PCM_Eval_Interface.h
