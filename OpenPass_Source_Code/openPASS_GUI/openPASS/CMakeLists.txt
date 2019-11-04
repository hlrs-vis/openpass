SET (SOURCES openPASS.cpp)
SET (HEADERS )
SET (INTERFACESOURCES )
SET (INTERFACEHEADERS Interfaces/openPASS/PluginInterface.h Interfaces/openPASS/PluginManagerInterface.h Interfaces/openPASS/RandomInterface.h Interfaces/openPASS/ServiceInterface.h Interfaces/openPASS/ServiceManagerInterface.h)

SET (MODELSSOURCES Models/PluginManagerModel.cpp Models/ServiceManagerModel.cpp)
SET (MODELSHEADERS Models/PluginManagerModel.h Models/ServiceManagerModel.h)


include_directories(../../OpenPASS/Common ../../Interfaces ../CoreShare importer framework)


add_library(openPASSInterfaces SHARED ${INTERFACESOURCES} ${INTERFACEHEADERS} )
qt5_use_modules(openPASSInterfaces Core Gui Widgets Xml)
install(TARGETS openPASSInterfaces
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )

add_executable(openPASS ${SOURCES} ${HEADERS} ${MODELSSOURCES} ${MODELSHEADERS} openPASS.rc)

target_link_libraries(openPASS CoreShare openPASSInterfaces)

qt5_use_modules(openPASS Core Gui Widgets Xml)

install(TARGETS openPASS
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")