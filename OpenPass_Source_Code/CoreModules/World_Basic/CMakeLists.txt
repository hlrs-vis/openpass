SET (SOURCES world_basic.cpp world_basic_implementation.cpp agentAdapter.cpp agentNetwork.cpp)
SET (HEADERS world_basic.h world_basic_implementation.h world_basic_global.h agentAdapter.h agentNetwork.h)


add_definitions(-DWORLD_BASIC_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(World_Basic SHARED ${SOURCES} ${HEADERS})
target_link_libraries(World_Basic Common)

qt5_use_modules(World_Basic Xml)

install(TARGETS World_Basic
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
