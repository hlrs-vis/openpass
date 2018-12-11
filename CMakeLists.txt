
#
# Root-file for the OpenPASS build process
#
#

# requirements to cmake version

# 2.8.7 is too old for identifying clang
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


# allow only out-of-source builds
STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" in_source_build)
IF(in_source_build)
  MESSAGE(FATAL_ERROR "A separate build directory is required, please create one and run cmake from this directory")
ENDIF(in_source_build)

# covise project
PROJECT(OpenPASS CXX C)

IF(POLICY CMP0020)
       #qt policy Automatically link Qt executables to qtmain target on Windows.
       cmake_policy(SET CMP0020 NEW)
ENDIF()
cmake_policy(SET CMP0043 NEW)
cmake_policy(SET CMP0054 NEW)

# this is used to override some of cmake's original find-modules
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH $ENV{EXTERNLIBS}/osi ${CMAKE_PREFIX_PATH})
find_package(OSI)
set(CMAKE_PREFIX_PATH $ENV{EXTERNLIBS}/qt5 ${CMAKE_PREFIX_PATH})
find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Xml)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5Gui)
find_package(Qt5Svg)
find_package(Qt5PrintSupport)
find_package(Qt5UiTools)
find_package(Qt5Script)
find_package(Qt5ScriptTools)

# this is used to override some of cmake's original find-modules
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

#######################################

IF(WIN32)
  SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/windows;${CMAKE_MODULE_PATH}")
ENDIF()
IF(UNIX)
  SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/unix;${CMAKE_MODULE_PATH}")
ENDIF()
IF(APPLE)
  SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/apple;${CMAKE_MODULE_PATH}")
ENDIF()
IF(MINGW)
  SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/mingw;${CMAKE_MODULE_PATH}")
ENDIF(MINGW)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_CFG_DIR "${CMAKE_INSTALL_PREFIX}/cfg" CACHE PATH "Installation directory for config files")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif()


# descend to sources
ADD_SUBDIRECTORY(OpenPass_Source_Code)
