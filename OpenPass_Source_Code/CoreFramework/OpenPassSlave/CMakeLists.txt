SET (SOURCES 
framework/frameworkConfig.cpp 
framework/agentFactory.cpp 
framework/observationModule.cpp 
framework/observationNetwork.cpp 
framework/runInstantiator.cpp 
framework/main.cpp 
importer/frameworkConfigImporter.cpp
importer/agentTypeImporter.cpp
importer/road.cpp
importer/runConfig.cpp
importer/runConfigImporter.cpp
importer/scenery.cpp
importer/sceneryImporter.cpp
modelElements/agent.cpp
modelElements/agentType.cpp
modelElements/channel.cpp
modelElements/component.cpp
modelElements/componentType.cpp
modelElements/spawnPointNetwork.cpp
modelInterface/modelBinding.cpp
modelInterface/modelLibrary.cpp
observationInterface/observationBinding.cpp
observationInterface/observationLibrary.cpp
spawnPointInterface/spawnPointBinding.cpp
spawnPointInterface/spawnPointLibrary.cpp
stochasticsInterface/stochastics.cpp
stochasticsInterface/stochasticsBinding.cpp
stochasticsInterface/stochasticsLibrary.cpp
worldInterface/world.cpp
worldInterface/worldBinding.cpp
worldInterface/worldLibrary.cpp
scheduler/runResult.cpp
scheduler/scheduleItem.cpp
scheduler/schedulePolicy.cpp
scheduler/scheduler.cpp
collisionDetectionInterface/collisionDetection.cpp
collisionDetectionInterface/collisionDetectionBinding.cpp
collisionDetectionInterface/collisionDetectionLibrary.cpp
)
SET (HEADERS 
framework/frameworkConfig.h 
framework/agentFactory.h 
framework/observationModule.h 
framework/observationNetwork.h 
framework/runInstantiator.h 
OpenPassSlave.h
importer/frameworkConfigImporter.h
importer/agentTypeImporter.h
importer/road.h
importer/runConfig.h
importer/runConfigImporter.h
importer/scenery.h
importer/sceneryImporter.h
modelElements/agent.h
modelElements/agentType.h
modelElements/channel.h
modelElements/channelBuffer.h
modelElements/component.h
modelElements/componentType.h
modelElements/spawnItemParameter.h
modelElements/spawnPoint.h
modelElements/spawnPointNetwork.h
modelInterface/modelBinding.h
modelInterface/modelLibrary.h
spawnPointInterface/spawnPointBinding.h
spawnPointInterface/spawnPointLibrary.h
stochasticsInterface/stochastics.h
stochasticsInterface/stochasticsBinding.h
stochasticsInterface/stochasticsLibrary.h
worldInterface/world.h
worldInterface/worldBinding.h
worldInterface/worldLibrary.h
scheduler/runResult.h
scheduler/scheduleItem.h
scheduler/schedulePolicy.h
scheduler/scheduler.h
observationInterface/observationBinding.h
observationInterface/observationLibrary.h
collisionDetectionInterface/collisionDetection.h
collisionDetectionInterface/collisionDetectionBinding.h
collisionDetectionInterface/collisionDetectionLibrary.h
)


include_directories(../../Common ../../Interfaces ../../Interfaces/roadInterface ../CoreShare importer worldInterface framework spawnPointInterface stochasticsInterface scheduler observationInterface collisionDetectionInterface modelElements modelInterface)

add_executable(OpenPassSlave ${SOURCES} ${HEADERS})

target_link_libraries(OpenPassSlave CoreShare Common)

qt5_use_modules(OpenPassSlave Widgets Xml)

install(TARGETS OpenPassSlave
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
