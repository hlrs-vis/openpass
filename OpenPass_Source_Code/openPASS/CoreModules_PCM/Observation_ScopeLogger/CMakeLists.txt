SET (SOURCES observation_scopeLogger.cpp observation_scopeLogger_implementation.cpp)
SET (HEADERS observation_scopeLogger.h observation_scopeLogger_global.h observation_scopeLogger_implementation.h)


add_definitions(-DOBSERVATION_SCOPELOGGER_LIBRARY)
include_directories(../../Common ../../Common_PCM ../../Interfaces)

add_library(Observation_ScopeLogger SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Observation_ScopeLogger Common Common_PCM)

qt5_use_modules(Observation_ScopeLogger Xml)

install(TARGETS Observation_ScopeLogger
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
