SET (SOURCES PluginPcm.cpp 
Models/ModelPcm.cpp
Models/FileHelper.cpp 
Models/ConfigurationGeneratorPcm/ConfigGeneratorPcm.cpp
Models/ConfigurationGeneratorPcm/ConfigWriter.cpp
Models/ConfigurationGeneratorPcm/XmlMergeHelper.cpp
Models/ConfigurationGeneratorPcm/DatabaseReaderPcm.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgent.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlTrajectory.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgentEquipment.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlSpawnPoint.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponent.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlScenery.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentObservation.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlRunConfig.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentSignal.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlPoint.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlGlobalData.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlParameter.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlIntendedCourses.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlObservation.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlLine.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlObject.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlMarks.cpp
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlViewObject.cpp
Views/ViewPcm.cpp
Presenters/PresenterPcm.cpp
)
SET (HEADERS PluginPcm.h 
Models/ModelPcm.h
Models/FileHelper.h
Models/ConfigurationGeneratorPcm/ConfigGeneratorPcm.h
Models/ConfigurationGeneratorPcm/ConfigWriter.h
Models/ConfigurationGeneratorPcm/DatabaseReaderPcm.h
Models/ConfigurationGeneratorPcm/XmlMergeHelper.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlLine.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlObject.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlIntendedCourses.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlObservation.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlGlobalData.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlParameter.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentSignal.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlPoint.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponentObservation.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlRunConfig.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlComponent.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlScenery.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgentEquipment.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlSpawnPoint.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlAgent.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlTrajectory.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlMarks.h
Models/ConfigurationGeneratorPcm/DataStructuresXml/XmlViewObject.h
Views/ViewPcm.h
Presenters/PresenterPcm.h
Interfaces/openPASS-PCM/PCMInterface.h
)

SET(UIS 
Views/ViewPcm.ui
)


add_definitions(-DPcm_LIBRARY)
include_directories(../../openPASS/Common ../../openPASS/Common_PCM ../../openPASS/Common_PCM/PCM_Data ../../openPASS/Common_PCM/DataStructuresBase ../../openPASS/Common_PCM/DataStructuresXosc ../../openPASS/Common_PCM/XoscWriter ../../Interfaces Interfaces ./ Models/ConfigurationGeneratorPcm Models/ConfigurationGeneratorPcm/DataStructuresXml)

add_library(Pcm SHARED ${SOURCES} ${HEADERS} ${UIS})
target_link_libraries(Pcm Common_PCM openPASSInterfaces)

qt5_use_modules(Pcm Xml Widgets Concurrent Sql)

install(TARGETS Pcm
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
