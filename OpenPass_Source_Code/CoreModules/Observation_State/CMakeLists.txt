SET (SOURCES observation_state.cpp observation_state_implementation.cpp)
SET (HEADERS observation_state.h observation_state_implementation.h observation_state_global.h observationtypes.h)


add_definitions(-DOBSERVATION_STATE_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(observation_state SHARED ${SOURCES} ${HEADERS})
target_link_libraries(observation_state Common)

qt5_use_modules(observation_state Xml)

install(TARGETS observation_state
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")