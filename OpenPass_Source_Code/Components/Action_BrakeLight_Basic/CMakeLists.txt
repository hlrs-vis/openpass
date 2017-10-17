SET (SOURCES action_brakelight_basic.cpp action_brakelight_basic_implementation.cpp)
SET (HEADERS action_brakelight_basic.h action_brakelight_basic_implementation.h action_brakelight_basic_global.h)


add_definitions(-DACTION_BRAKELIGHT_BASIC_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Action_BrakeLight_Basic SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Action_BrakeLight_Basic Xml)

install(TARGETS Action_BrakeLight_Basic
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")