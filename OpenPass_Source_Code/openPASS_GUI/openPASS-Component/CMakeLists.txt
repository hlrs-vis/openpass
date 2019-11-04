SET (SOURCES ComponentPlugin.cpp 
Models/ComponentInputItemModel.cpp
Models/ComponentXMLSaveModel.cpp
Models/ComponentInputMapModel.cpp
Models/ComponentXMLLoadModel.cpp
Models/ComponentItemModel.cpp
Models/ComponentXMLBaseModel.cpp
Models/ComponentMapModel.cpp
Models/ComponentTypedefMapModel.cpp
Models/ComponentModel.cpp
Models/ComponentTypedefItemModel.cpp
Models/ComponentOutputItemModel.cpp
Models/ComponentScheduleModel.cpp
Models/ComponentOutputMapModel.cpp
Models/ComponentParameterMapModel.cpp
Models/ComponentParameterItemModel.cpp 
)
SET (HEADERS ComponentPlugin.h 
Models/ComponentParameterItemModel.h
Models/ComponentOutputMapModel.h
Models/ComponentParameterMapModel.h
Models/ComponentOutputItemModel.h
Models/ComponentScheduleModel.h
Models/ComponentModel.h
Models/ComponentTypedefItemModel.h
Models/ComponentMapModel.h
Models/ComponentTypedefMapModel.h
Models/ComponentItemModel.h
Models/ComponentXMLBaseModel.h
Models/ComponentInputMapModel.h
Models/ComponentXMLLoadModel.h
Models/ComponentInputItemModel.h
Models/ComponentXMLSaveModel.h
Interfaces/openPASS-Component/ComponentInputItemInterface.h
Interfaces/openPASS-Component/ComponentInputMapInterface.h
Interfaces/openPASS-Component/ComponentInterface.h
Interfaces/openPASS-Component/ComponentItemInterface.h
Interfaces/openPASS-Component/ComponentMapInterface.h
Interfaces/openPASS-Component/ComponentOutputItemInterface.h
Interfaces/openPASS-Component/ComponentOutputMapInterface.h
Interfaces/openPASS-Component/ComponentParameterItemInterface.h
Interfaces/openPASS-Component/ComponentParameterMapInterface.h
Interfaces/openPASS-Component/ComponentScheduleInterface.h
Interfaces/openPASS-Component/ComponentTypedefItemInterface.h
Interfaces/openPASS-Component/ComponentTypedefMapInterface.h
)



add_definitions(-DCOMPONENT_LIBRARY)
include_directories(../../openPASS/Common ../../Interfaces Interfaces)

add_library(Component SHARED ${SOURCES} ${HEADERS} ${UIS})
target_link_libraries(Component openPASSInterfaces)

qt5_use_modules(Component Xml Widgets)

install(TARGETS Component
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")
