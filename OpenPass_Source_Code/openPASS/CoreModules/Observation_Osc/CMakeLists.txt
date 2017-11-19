SET (SOURCES observation_osc.cpp observation_osc_implementation.cpp)
SET (HEADERS observation_osc_global.h  observation_osc_implementation.h observation_osc.h observationtypes.h)


add_definitions(-DOBSERVATION_OSC_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Observation_Osc SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Observation_Osc Common)

qt5_use_modules(Observation_Osc Xml)

install(TARGETS Observation_Osc
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
