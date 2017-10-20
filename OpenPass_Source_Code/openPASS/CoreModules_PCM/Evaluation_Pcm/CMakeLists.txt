SET (SOURCES evaluation_pcm.cpp evaluation_pcm_implementation.cpp collisionDetection.cpp runResult.cpp)
SET (HEADERS evaluation_pcm.h evaluation_pcm_implementation.h evaluation_pcm_global.h collisionDetection.h agent.h runResult.h)


add_definitions(-DEVALUATION_PCM_LIBRARY)
include_directories(../../Common ../../Common_PCM ../../Interfaces)

add_library(Evaluation_Pcm SHARED ${SOURCES} ${HEADERS})
target_link_libraries(Evaluation_Pcm Common)

qt5_use_modules(Evaluation_Pcm Xml)

install(TARGETS Evaluation_Pcm
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
