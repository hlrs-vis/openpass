SET (SOURCES SimulationPlugin.cpp
Models/SimulationModel.cpp
Views/SimulationView.cpp
Presenters/SimulationPresenter.cpp
)
SET (HEADERS SimulationPlugin.h 
Models/SimulationModel.h
Views/SimulationView.h
Presenters/SimulationPresenter.h
)

SET(UIS 
Views/SimulationView.ui
)


add_definitions(-DSimulation_LIBRARY)
include_directories(../../OpenPASS/Common ../../Interfaces Interfaces ./ ../openPASS-Project/Interfaces)

add_library(Simulation SHARED ${SOURCES} ${HEADERS} ${UIS})
target_link_libraries(Simulation Project)

qt5_use_modules(Simulation Xml Widgets)

install(TARGETS Simulation
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
