SET (SOURCES SceneryPlugin.cpp
Models/SceneryModel.cpp
Views/SceneryView.cpp
Presenters/SceneryPresenter.cpp
)
SET (HEADERS SceneryPlugin.h 
Models/SceneryModel.h
Views/SceneryView.h
Presenters/SceneryPresenter.h
)

SET(UIS 
Views/SceneryView.ui
)


add_definitions(-DScenery_LIBRARY)
include_directories(../../OpenPASS/Common ../../Interfaces Interfaces ./ ../openPASS-Project/Interfaces)

add_library(Scenery SHARED ${SOURCES} ${HEADERS} ${UIS})

target_link_libraries(Scenery Project)

qt5_use_modules(Scenery Xml Widgets)

install(TARGETS Scenery
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
