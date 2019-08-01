message("***** PARSING defaults.pri *****")

win32 {
    isEmpty(DESTDIR) {
        DESTDIR_SLAVE="C:\OpenPASS\bin"
        DESTDIR_TESTS="C:\OpenPASS\bin\tests"
    } else {
        DESTDIR_SLAVE=$$DESTDIR
        DESTDIR_TESTS=$$DESTDIR
    }

    isEmpty(EXTRA_INCLUDE_PATH) {
        EXTRA_INCLUDE_PATH="$$PWD\thirdParty\include"
    }

    isEmpty(EXTRA_LIB_PATH) {
        EXTRA_LIB_PATH="$$PWD\thirdParty\lib\win"
    }
}

unix {
    isEmpty(DESTDIR) {
        DESTDIR_SLAVE="/OpenPASS/bin"
        DESTDIR_TESTS="/OpenPASS/bin/tests"
    } else {
        DESTDIR_SLAVE=$$DESTDIR
        DESTDIR_TESTS=$$DESTDIR
    }

    isEmpty(EXTRA_INCLUDE_PATH) {
        EXTRA_INCLUDE_PATH="$$PWD/thirdParty/include"
    }

    isEmpty(EXTRA_LIB_PATH) {
        EXTRA_LIB_PATH=$$PWD/thirdParty/lib
    }
}
