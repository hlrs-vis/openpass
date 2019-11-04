SET (SOURCES AgentPlugin.cpp Models/AgentModel.cpp Presenters/AgentPresenter.cpp Views/AgentView.cpp)
SET (HEADERS AgentPlugin.h Interfaces/openPASS-Agent/AgentInterface.h Models/AgentModel.h Presenters/AgentPresenter.h Views/AgentView.h)
SET (UIS Views/AgentView.ui)


add_definitions(-DAGENT_LIBRARY)
include_directories(../../openPASS/Common ../../Interfaces)

add_library(Agent SHARED ${SOURCES} ${HEADERS} ${UIS})
target_link_libraries(Agent openPASSInterfaces)

qt5_use_modules(Agent Xml Widgets)

install(TARGETS Agent
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
