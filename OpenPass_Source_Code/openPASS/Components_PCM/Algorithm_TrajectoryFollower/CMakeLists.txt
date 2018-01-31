SET (SOURCES algorithm_trajectoryFollower.cpp algorithm_trajectoryFollower_implementation.cpp trajectoryfollowing.cpp)
SET (HEADERS algorithm_trajectoryFollower_global.h algorithm_trajectoryFollower.h algorithm_trajectoryFollower_implementation.h trajectoryfollowing.h)


add_definitions(-DALGORITHM_TRAJECTORYFOLLOWER_LIBRARY)
include_directories(../../Common_PCM ../../Interfaces)

add_library(Algorithm_TrajectoryFollower SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(Algorithm_TrajectoryFollower Xml)

install(TARGETS Algorithm_TrajectoryFollower
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
