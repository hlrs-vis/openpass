SET (SOURCES sensor_distance.cpp sensor_distance_implementation.cpp)
SET (HEADERS sensor_distance.h sensor_distance_implementation.h sensor_distance_global.h)


add_definitions(-DSENSOR_DISTANCE_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Sensor_Distance SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Sensor_Distance Xml)

install(TARGETS Sensor_Distance
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
