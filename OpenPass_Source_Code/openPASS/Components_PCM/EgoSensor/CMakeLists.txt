SET (SOURCES egoSensor.cpp egoSensor_implementation.cpp)
SET (HEADERS egoSensor_global.h egoSensor.h egoSensor_implementation.h)


add_definitions(-DEGO_SENSOR_LIBRARY)
include_directories(../../Common_PCM ../../Common ../../Interfaces)

add_library(EgoSensor SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(EgoSensor Xml)

install(TARGETS EgoSensor
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
