SET (SOURCES collisionDetection_Impact.cpp collisionDetection_Impact_implementation.cpp polygon.cpp)
SET (HEADERS collisionDetection_Impact_global.h collisionDetection_Impact_implementation.h polygon.h collisionDetection_Impact.h postCrashDynamic.h)


add_definitions(-DEVALUATION_PCM_LIBRARY)
include_directories(../../Common ../../Common_PCM ../../Interfaces)

add_library(CollisionDetection_Impact SHARED ${SOURCES} ${HEADERS})
target_link_libraries(CollisionDetection_Impact Common)

qt5_use_modules(CollisionDetection_Impact Xml)

install(TARGETS CollisionDetection_Impact
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
