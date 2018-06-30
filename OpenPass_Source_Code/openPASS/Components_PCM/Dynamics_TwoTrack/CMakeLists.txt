SET (SOURCES dynamics_twotrack.cpp dynamics_twotrack_implementation.cpp dynamics_twotrack_local.cpp dynamics_twotrack_tire.cpp dynamics_twotrack_vehicle.cpp)
SET (HEADERS dynamics_twotrack_global.h dynamics_twotrack.h dynamics_twotrack_implementation.h dynamics_twotrack_local.h dynamics_twotrack_tire.h dynamics_twotrack_vehicle.h)

add_definitions(-DDYNAMICS_TWOTRACK_LIBRARY)
include_directories(../../Common_PCM ../../Common ../../Interfaces)

add_library(Dynamics_TwoTrack SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Dynamics_TwoTrack Common)

qt5_use_modules(Dynamics_TwoTrack Xml)

install(TARGETS Dynamics_TwoTrack
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
