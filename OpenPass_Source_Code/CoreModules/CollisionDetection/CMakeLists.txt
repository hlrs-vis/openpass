SET (SOURCES collisionDetection.cpp collisionDetection_implementation.cpp)
SET (HEADERS collisionDetection.h collisionDetection_implementation.h collisionDetection_global.h)


add_definitions(-DCOLLISIONDETECTION_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(CollisionDetection SHARED ${SOURCES} ${HEADERS})
target_link_libraries(CollisionDetection Common)

qt5_use_modules(CollisionDetection Xml)

install(TARGETS CollisionDetection
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")