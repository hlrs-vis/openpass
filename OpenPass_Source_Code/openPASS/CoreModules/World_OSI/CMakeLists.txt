SET (SOURCES AgentAdapter.cpp AgentNetwork.cpp CommonSceneryHelper.cpp GeometryConverter.cpp SceneryConverter.cpp World.cpp WorldData.cpp WorldDataException.cpp WorldImplementation.cpp WorldObjectAdapter.cpp)
SET (HEADERS AgentAdapter.h AgentNetwork.h CommonSceneryHelper.h GeometryConverter.h SceneryConverter.h World.h WorldData.h WorldDataException.h WorldGlobal.h WorldImplementation.h WorldObjectAdapter.h)
SET (LOCALIZATIONSOURCES Localization/GeometryProcessor.cpp Localization/LaneWalker.cpp Localization/LaneWalker.cpp Localization/Localization.cpp Localization/PointAggregator.cpp Localization/PointLocator.cpp Localization/PointQuery.cpp Localization/SearchInitializer.cpp Localization/SectionObjectGenerator.cpp)
SET (LOCALIZATIONHEADERS Localization/GeometryProcessor.h Localization/LaneWalker.h Localization/LaneWalker.h Localization/Localization.h Localization/PointAggregator.h Localization/PointLocator.h Localization/PointQuery.h Localization/SearchInitializer.h Localization/SectionObjectGenerator.h Localization/LocalizationCommon.h)
SET (OWLSOURCES OWL/DataTypes.cpp OWL/OpenDriveTypeMapper.cpp )
SET (OWLHEADERS OWL/DataTypes.h OWL/LaneGeometryElement.h OWL/LaneGeometryJoint.h OWL/OpenDriveTypeMapper.h OWL/Primitives.h)

find_package(OSI)
find_package(Protobuf 2.6.1 REQUIRED)

add_definitions(-DWORLD_OSI_LIBRARY -DWORLD_LIBRARY -DPROTOBUF_USE_DLLS)
include_directories(../../Common ../../Interfaces ../../Interfaces/roadInterface Localization OWL ./ ${OSI_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIRS})

add_library(World_OSI SHARED ${SOURCES} ${HEADERS} ${LOCALIZATIONSOURCES} ${LOCALIZATIONHEADERS} ${OWLSOURCES} ${OWLHEADERS})
target_link_libraries(World_OSI Common  ${OSI_LIBRARIES} ${PROTOBUF_LIBRARIES})

qt5_use_modules(World_OSI Xml)

install(TARGETS World_OSI
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
