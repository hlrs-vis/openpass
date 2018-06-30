SET (SOURCES sensor_collision.cpp sensor_collision_implementation.cpp)
SET (HEADERS sensor_collision_global.h sensor_collision.h sensor_collision_implementation.h)


add_definitions(-DSENSOR_COLLISION_LIBRARY)
include_directories(../../Common_PCM  ../../CoreModules_PCM/CollisionDetection_Impact ../../Common ../../Interfaces)

add_library(Sensor_Collision SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Sensor_Collision Xml)

install(TARGETS Sensor_Collision
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
