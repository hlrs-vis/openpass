SET (SOURCES WindowPlugin.cpp
Models/WindowModel.cpp
Views/WindowView.cpp
Views/Views-openPASS/LogoDelegate.cpp
Presenters/WindowPresenter.cpp
)
SET (HEADERS WindowPlugin.h 
Models/WindowModel.h
Views/WindowView.h
Views/LogoDelegate.h
Presenters/WindowPresenter.h
Interfaces/openPASS-Window/WindowInterface.h
)

SET(UIS 
Views/WindowView.ui
)


add_definitions(-DWindow_LIBRARY)
include_directories(../../OpenPASS/Common ../../Interfaces Interfaces ./)

add_library(Window SHARED ${SOURCES} ${HEADERS} ${UIS})

target_link_libraries(Window openPASSInterfaces)

qt5_use_modules(Window Xml Widgets)

install(TARGETS Window
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
