SET (SOURCES PCM_Data/pcm_agent.cpp
PCM_Data/pcm_course.cpp
PCM_Data/pcm_data.cpp
PCM_Data/pcm_globalData.cpp
PCM_Data/pcm_initialValues.cpp
PCM_Data/pcm_intendedCourse.cpp
PCM_Data/pcm_line.cpp
PCM_Data/pcm_lineContainer.cpp
PCM_Data/pcm_lineSegment.cpp
PCM_Data/pcm_marks.cpp
PCM_Data/pcm_object.cpp
PCM_Data/pcm_participantData.cpp
PCM_Data/pcm_point.cpp
PCM_Data/pcm_pointContainer.cpp
PCM_Data/pcm_trajectory.cpp
PCM_Data/pcm_viewObject.cpp
PCM_Importer/scenarioImporterPCM.cpp
PCM_Importer/sceneryImporterPCM.cpp
DataStructuresBase/XmlBaseClass.cpp
DataStructuresXosc/XoscScenario.cpp
DataStructuresXosc/XoscTrajectory.cpp
XoscWriter/openScenarioWriter.cpp
)
SET (HEADERS PCM_Data/pcm_agent.h
PCM_Data/pcm_course.h
PCM_Data/pcm_data.h
PCM_Data/pcm_globalData.h
PCM_Data/pcm_helper.h
PCM_Data/pcm_initialValues.h
PCM_Data/pcm_intendedCourse.h
PCM_Data/pcm_line.h
PCM_Data/pcm_lineContainer.h
PCM_Data/pcm_lineSegment.h
PCM_Data/pcm_marks.h
PCM_Data/pcm_object.h
PCM_Data/pcm_participantData.h
PCM_Data/pcm_point.h
PCM_Data/pcm_pointContainer.h
PCM_Data/pcm_trajectory.h
PCM_Data/pcm_viewObject.h
PCM_Importer/scenarioImporterPCM.h
PCM_Importer/sceneryImporterPCM.h
DataStructuresBase/XmlBaseClass.h
DataStructuresXosc/XoscScenario.h
DataStructuresXosc/XoscTrajectory.h
XoscWriter/openScenarioWriter.h
)

include_directories(../Common PCM_Data ./)

add_library(Common_PCM SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Common_PCM Common)

qt5_use_modules(Common_PCM Core Xml)

install(TARGETS Common_PCM
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")