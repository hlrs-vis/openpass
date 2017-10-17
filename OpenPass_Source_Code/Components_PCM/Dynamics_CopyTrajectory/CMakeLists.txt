SET (SOURCES Dynamics_CopyTrajectory.cpp Dynamics_CopyTrajectory_implementation.cpp)
SET (HEADERS Dynamics_CopyTrajectory.h Dynamics_CopyTrajectory_implementation.h Dynamics_CopyTrajectory_global.h)


add_definitions(-DDYNAMICS_COPYTRAJECTORY_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Dynamics_CopyTrajectory SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Dynamics_CopyTrajectory Xml)

install(TARGETS Dynamics_CopyTrajectory
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")