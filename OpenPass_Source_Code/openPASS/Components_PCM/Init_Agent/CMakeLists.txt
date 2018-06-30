SET (SOURCES init_agent.cpp init_agent_implementation.cpp)
SET (HEADERS init_agent_global.h init_agent.h init_agent_implementation.h)


add_definitions(-DINIT_AGENT_LIBRARY)
include_directories(../../Common_PCM ../../Common ../../Interfaces)

add_library(Init_Agent SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Init_Agent Xml)

install(TARGETS Init_Agent
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
