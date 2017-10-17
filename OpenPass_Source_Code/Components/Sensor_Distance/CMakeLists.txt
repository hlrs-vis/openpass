SET (SOURCES Sensor_Distance.cpp Sensor_Distance_implementation.cpp)
SET (HEADERS Sensor_Distance.h Sensor_Distance_implementation.h Sensor_Distance_global.h)


add_definitions(-DSENSOR_DISTANCE_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Sensor_Distance SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Sensor_Distance Xml)

install(TARGETS Sensor_Distance
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")