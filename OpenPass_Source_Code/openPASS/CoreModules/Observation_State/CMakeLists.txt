SET (SOURCES observation_state.cpp observation_state_implementation.cpp)
SET (HEADERS observation_state.h observation_state_implementation.h observation_state_global.h observationtypes.h)


add_definitions(-DOBSERVATION_STATE_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Observation_State SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Observation_State Common)

qt5_use_modules(Observation_State Xml)

install(TARGETS Observation_State
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
