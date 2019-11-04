SET (SOURCES algorithm_Selector.cpp algorithm_Selector_implementation.cpp selector.cpp)
SET (HEADERS algorithm_Selector_global.h algorithm_Selector.h algorithm_Selector_implementation.h selector.h)


add_definitions(-DALGORITHM_SELECTOR_LIBRARY)
include_directories(../../Common_PCM ../../Common_PCM/PCM_Data ../../Common_PCM/PCM_Importer ../../Common ../../Interfaces)

add_library(Algorithm_Selector SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Algorithm_Selector Common Common_PCM)

qt5_use_modules(Algorithm_Selector Xml Widgets)

install(TARGETS Algorithm_Selector
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
