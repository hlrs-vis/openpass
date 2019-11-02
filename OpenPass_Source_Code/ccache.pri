message("***** PARSING ccache.pri *****")

CONFIG(USE_CCACHE): {
    message("[$$TARGET] Activate 'ccache'")

    win32 {
        system("ccache --version > NUL"): {
            # ccache is located in PATH
            CCACHE_BIN="ccache"
        }
    }

    unix {
        # ccache is located in PATH
        system("ccache --version > /dev/null") {
            CCACHE_BIN="ccache"
        }
    }

    isEmpty(CCACHE_BIN) {
        error("USE_CCACHE is set, but couldn't locate ccache executable")
    } else {
        QMAKE_CC=$$CCACHE_BIN $$QMAKE_CC
        QMAKE_CXX=$$CCACHE_BIN $$QMAKE_CXX
    }
}
