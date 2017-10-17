SET (SOURCES stochastics.cpp stochastics_implementation.cpp)
SET (HEADERS stochastics.h stochastics_implementation.h stochastics_global.h)


add_definitions(-DSTOCHASTICS_LIBRARY)
include_directories(../../Common ../../Interfaces)

add_library(Stochastics SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Stochastics Common)

qt5_use_modules(Stochastics Xml)

install(TARGETS Stochastics
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
