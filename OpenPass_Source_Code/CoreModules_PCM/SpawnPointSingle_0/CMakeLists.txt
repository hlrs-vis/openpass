SET (SOURCES spawnPointSingle_0.cpp spawnPointSingle_0_implementation.cpp )
SET (HEADERS spawnPointSingle_0.h spawnPointSingle_0_implementation.h spawnPointSingle_0_global.h )


add_definitions(-DSPAWNPOINTSINGLE_0_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(SpawnPointSingle_0 SHARED ${SOURCES} ${HEADERS})
target_link_libraries(SpawnPointSingle_0 Common)

qt5_use_modules(SpawnPointSingle_0 Xml)

install(TARGETS SpawnPointSingle_0
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
