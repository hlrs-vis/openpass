SET (SOURCES agentAdapter.cpp  agentNetwork.cpp world_pcm_implementation.cpp world_pcm.cpp)
SET (HEADERS world_pcm_global.h agentAdapter.h    agentNetwork.h world_pcm.h         world_pcm_implementation.h)


add_definitions(-DEVALUATION_PCM_LIBRARY)
include_directories(../../Common ../../Common_PCM PCM_Data ../../Interfaces)

add_library(World_PCM SHARED ${SOURCES} ${HEADERS})
target_link_libraries(World_PCM Common)

qt5_use_modules(World_PCM Xml)

install(TARGETS World_PCM
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
