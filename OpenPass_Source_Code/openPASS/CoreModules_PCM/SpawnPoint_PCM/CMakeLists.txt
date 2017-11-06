SET (SOURCES spawnPoint_PCM.cpp spawnPoint_PCM_implementation.cpp )
SET (HEADERS spawnPoint_PCM.h spawnPoint_PCM_implementation.h )


add_definitions(-DSPAWNPOINT_PCM_LIBRARY)
include_directories(../../Common ../../Common_PCM ../../Interfaces)

add_library(SpawnPoint_PCM SHARED ${SOURCES} ${HEADERS})
target_link_libraries(SpawnPoint_PCM Common)

qt5_use_modules(SpawnPoint_PCM Xml)

install(TARGETS SpawnPoint_PCM
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
