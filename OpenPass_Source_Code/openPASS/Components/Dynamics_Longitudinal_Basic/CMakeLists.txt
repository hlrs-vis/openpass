SET (SOURCES dynamics_longitudinal_basic.cpp dynamics_longitudinal_basic_implementation.cpp)
SET (HEADERS dynamics_longitudinal_basic.h dynamics_longitudinal_basic_implementation.h dynamics_longitudinal_basic_global.h)


add_definitions(-DDYNAMICS_LONGITUDINAL_BASIC_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Dynamics_Longitudinal_Basic SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Dynamics_Longitudinal_Basic Xml)

install(TARGETS Dynamics_Longitudinal_Basic
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
