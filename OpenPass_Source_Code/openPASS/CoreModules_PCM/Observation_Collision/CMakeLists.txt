SET (SOURCES observation_collision.cpp observation_collision_implementation.cpp)
SET (HEADERS observation_collision_global.h observation_collision.h observation_collision_implementation.h)


add_definitions(-DEVALUATION_PCM_LIBRARY)
include_directories(../../Common ../../Common_PCM ../../Interfaces)

add_library(Observation_Collision SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Observation_Collision Common)

qt5_use_modules(Observation_Collision Xml)

install(TARGETS Observation_Collision
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
