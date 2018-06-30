SET (SOURCES dynamics_collision.cpp dynamics_collision_implementation.cpp)
SET (HEADERS dynamics_collision_global.h dynamics_collision.h dynamics_collision_implementation.h)


add_definitions(-DDYNAMICS_COLLISION_LIBRARY)
include_directories(../../Common_PCM ../../CoreModules_PCM/CollisionDetection_Impact ../../Common ../../Interfaces)

add_library(Dynamics_Collision SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Dynamics_Collision Common)

qt5_use_modules(Dynamics_Collision Xml)

install(TARGETS Dynamics_Collision
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
