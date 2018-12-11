SET (SOURCES vector2d.cpp vector3d.cpp utilities.cpp)
SET (HEADERS vector2d.h vector3d.h utilities.h carInfo.h commonTools.h globalDefinitions.h primitiveSignals.h opExport.h)

add_library(Common SHARED ${SOURCES} ${HEADERS})

install(TARGETS Common
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")