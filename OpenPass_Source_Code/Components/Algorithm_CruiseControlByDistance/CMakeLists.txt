SET (SOURCES algorithm_cruisecontrolbydistance.cpp algorithm_cruisecontrolbydistance_implementation.cpp)
SET (HEADERS algorithm_cruisecontrolbydistance.h algorithm_cruisecontrolbydistance_implementation.h algorithm_cruisecontrolbydistance_global.h)


add_definitions(-DALGORITHM_CRUISECONTROLBYDISTANCE_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Algorithm_cruisecontrolbydistance SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Algorithm_cruisecontrolbydistance Xml)

install(TARGETS Algorithm_cruisecontrolbydistance
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")