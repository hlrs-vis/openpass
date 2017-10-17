SET (SOURCES spawnpoint_start.cpp spawnpoint_start_implementation.cpp)
SET (HEADERS spawnpoint_start.h spawnpoint_start_implementation.h spawnpoint_start_global.h)


add_definitions(-DSPAWNPOINT_START_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(SpawnPoint_Start SHARED ${SOURCES} ${HEADERS})
target_link_libraries(SpawnPoint_Start Common)

qt5_use_modules(SpawnPoint_Start Xml)

install(TARGETS SpawnPoint_Start
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
