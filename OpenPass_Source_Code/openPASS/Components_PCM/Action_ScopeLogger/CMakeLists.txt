SET (SOURCES action_scopeLogger.cpp action_scopeLogger_implementation.cpp)
SET (HEADERS action_scopeLogger_global.h action_scopeLogger.h action_scopeLogger_implementation.h)


add_definitions(-DACTION_SCOPELOGGER_LIBRARY)
include_directories(../../Common_PCM ../../Common ../../Interfaces)

add_library(Action_ScopeLogger SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Action_ScopeLogger Xml)

install(TARGETS Action_ScopeLogger
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
