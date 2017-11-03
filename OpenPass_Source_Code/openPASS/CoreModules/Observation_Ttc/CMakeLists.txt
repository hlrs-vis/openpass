SET (SOURCES observation_ttc.cpp observation_ttc_implementation.cpp)
SET (HEADERS observation_ttc.h observation_ttc_implementation.h observation_ttc_global.h)


add_definitions(-DOBSERVATION_TTC_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Observation_Ttc SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Observation_Ttc Common)

qt5_use_modules(Observation_Ttc Xml)

install(TARGETS Observation_Ttc
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
