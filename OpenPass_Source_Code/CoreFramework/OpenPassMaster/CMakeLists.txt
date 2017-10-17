SET (SOURCES framework/frameworkConfig.cpp framework/main.cpp importer/frameworkConfigImporter.cpp)
SET (HEADERS framework/frameworkConfig.h openPassMaster.h importer/frameworkConfigImporter.h)


include_directories(../../Common ../../Interfaces ../CoreShare importer framework)

add_executable(OpenPassMaster ${SOURCES} ${HEADERS})

target_link_libraries(OpenPassMaster CoreShare)

qt5_use_modules(OpenPassMaster Widgets Xml)

install(TARGETS OpenPassMaster
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")