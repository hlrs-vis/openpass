SET (SOURCES 
agentAdapter.cpp 
agentNetwork.cpp 
world_pcm_implementation.cpp 
world_pcm.cpp 
PCM_Data/pcm_course.cpp 
PCM_Data/pcm_data.cpp 
PCM_Data/pcm_globalData.cpp 
PCM_Data/pcm_line.cpp 
PCM_Data/pcm_lineContainer.cpp 
PCM_Data/pcm_lineSegment.cpp 
PCM_Data/pcm_marks.cpp 
PCM_Data/pcm_object.cpp 
PCM_Data/pcm_point.cpp 
PCM_Data/pcm_pointContainer.cpp 
PCM_Data/pcm_viewObject.cpp 
PCM_Data/sceneryImporterPCM.cpp 
) 
SET (HEADERS 
world_pcm_global.h 
agentAdapter.h 
agentNetwork.h world_pcm.h 
world_pcm_implementation.h
PCM_Data/pcm_course.h 
PCM_Data/pcm_data.h 
PCM_Data/pcm_globalData.h 
PCM_Data/pcm_helper.h 
PCM_Data/pcm_line.h 
PCM_Data/pcm_lineContainer.h 
PCM_Data/pcm_lineSegment.h 
PCM_Data/pcm_marks.h 
PCM_Data/pcm_object.h 
PCM_Data/pcm_point.h 
PCM_Data/pcm_pointContainer.h 
PCM_Data/pcm_viewObject.h 
PCM_Data/sceneryImporterPCM.h 
)


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
