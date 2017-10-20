SET (SOURCES callbacks.cpp log.cpp parameterParser.cpp parameters.cpp xmlParser.cpp cephesMIT/const.c cephesMIT/fresnl.c cephesMIT/polevl.c)
SET (HEADERS callbacks.h log.h parameterParser.h parameters.h xmlParser.h  cephesMIT/mconf.h)


include_directories(../../Common ../../Interfaces)

add_library(CoreShare STATIC ${SOURCES} ${HEADERS})

qt5_use_modules(CoreShare Xml)

install(TARGETS CoreShare
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
		
install(FILES ${HEADERS} DESTINATION "${INSTALL_INC_DIR}")