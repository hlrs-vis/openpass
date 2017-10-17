SET (SOURCES spawnpoint_start.cpp spawnpoint_start_implementation.cpp)
SET (HEADERS spawnpoint_start.h spawnpoint_start_implementation.h spawnpoint_start_global.h)


add_definitions(-DSPAWNPOINT_START_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(spawnpoint_start SHARED ${SOURCES} ${HEADERS})
target_link_libraries(spawnpoint_start Common)

qt5_use_modules(spawnpoint_start Xml)

install(TARGETS spawnpoint_start
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")