SET (SOURCES ProjectPlugin.cpp
Models/ProjectXMLSaveModel.cpp
Models/ProjectModel.cpp
Models/ProjectXMLLoadModel.cpp
Models/ProjectXMLBaseModel.cpp
Views/ProjectView.cpp
Presenters/ProjectPresenter.cpp
)
SET (HEADERS ProjectPlugin.h 
Models/ProjectModel.h
Models/ProjectXMLLoadModel.h
Models/ProjectXMLSaveModel.h
Models/ProjectXMLBaseModel.h
Views/ProjectView.h
Presenters/ProjectPresenter.h
Interfaces/openPASS-Project/ProjectInterface.h
)

SET(UIS 
Views/ProjectView.ui
)


add_definitions(-DProject_LIBRARY)
include_directories(../../OpenPASS/Common ../../Interfaces Interfaces ./)

add_library(Project SHARED ${SOURCES} ${HEADERS} ${UIS})
target_link_libraries(Project openPASSInterfaces)

qt5_use_modules(Project Xml Widgets)

install(TARGETS Project
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
