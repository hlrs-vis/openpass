SET (SOURCES PluginPcm_Eval.cpp 
Models/ModelPcm_Eval.cpp
Models/TableModelCsv.cpp 
Views/TrafficScene.cpp
Views/TrajectoryGraphicsItem.cpp
Views/ViewPcm_Eval.cpp
Presenters/PresenterPcm_Eval.cpp
)
SET (HEADERS PluginPcm_Eval.h 
Models/ModelPcm_Eval.h
Models/TableModelCsv.h
Views/TrafficScene.h
Views/TrajectoryGraphicsItem.h
Views/ViewPcm_Eval.h
Presenters/PresenterPcm_Eval.h
Interfaces/openPASS-Pcm_Eval/Pcm_Eval_Interface.h
)

SET(UIS 
Views/ViewPcm_Eval.ui
)


add_definitions(-DPcm_Eval_LIBRARY)
include_directories(../../openPASS/Common ../../openPASS/Common_PCM ../../openPASS/Common_PCM/PCM_Importer ../../openPASS/Common_PCM/PCM_Data ../../Interfaces Interfaces ./)

add_library(Pcm_Eval SHARED ${SOURCES} ${HEADERS} ${UIS})

target_link_libraries(Pcm_Eval Common_PCM openPASSInterfaces)
qt5_use_modules(Pcm_Eval Xml Widgets)

install(TARGETS Pcm_Eval
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
