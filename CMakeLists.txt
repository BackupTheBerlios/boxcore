cmake_minimum_required(VERSION 2.6)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

project (boxCore)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)
set (VERSION_PATCH 0)
set (VERSION_BUILD 0)

set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

option(USE_UNICOWS "Should we link against unicows.dll?" ON)

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH amd64)
set (PACKAGE_ARCH x64)
set (WINAPI_DECORATOR "")
# set (UNICOWSLIB "")
else (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH x86)
set (PACKAGE_ARCH x86)
set (WINAPI_DECORATOR "@8")
if (USE_UNICOWS)
add_library(unicows STATIC IMPORTED)
set_target_properties (unicows PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libunicows.a)
set (UNICOWSLIB "unicows")
else (USE_UNICOWS)
set (UNICOWSLIB "")
endif (USE_UNICOWS)
endif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)

find_package(Doxygen)

if (DOXYGEN_EXECUTABLE)
set (DOXYGEN_HHC_EXECUTABLE "hhc" CACHE FILEPATH "Path to the hhc program")
if (DOXYGEN_DOT_EXECUTABLE)
set (USE_DOT "YES")
else (DOXYGEN_DOT_EXECUTABLE)
set (USE_DOT "NO")
endif (DOXYGEN_DOT_EXECUTABLE)

option (INTERNALDOCS "Generate internal project documentation?" OFF)
option (DOCS_CHM "Generate a .chm file?" OFF)
option (DEBUG_CODE "Activate code for generating debug messages" OFF)

if (DOCS_CHM)
set (DOXYGEN_HHC ${DOXYGEN_HHC_EXECUTABLE})
endif (DOCS_CHM)

if (INTERNALDOCS)
set (INTERNAL_DOCS "YES")
else (INTERNALDOCS)
set (INTERNAL_DOCS "NO")
endif (INTERNALDOCS)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
add_custom_target(doc ${DOXYGEN_EXECUTABLE})
endif (DOXYGEN_EXECUTABLE)

find_program(ASTYLE_EXECUTABLE astyle 
             NO_CMAKE_ENVIRONMENT_PATH
             NO_CMAKE_PATH)
if (ASTYLE_EXECUTABLE)
add_custom_target(astyle
                  COMMAND ${ASTYLE_EXECUTABLE} --options=astyleconfig "*.cpp" "*.h"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
                  COMMENT "Formatting source files...")
endif (ASTYLE_EXECUTABLE)

set (CPACK_ALL_INSTALL_TYPES Full Upgrade)

add_subdirectory (debug)

add_subdirectory (blackbox)

install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/styles DESTINATION . COMPONENT Styles)

set (CPACK_SOURCE_GENERATOR "ZIP")
set (CPACK_SOURCE_IGNORE_FILES ".hg/;build*;styles/*;config/*;.hgtags;.hgignore;bb4win64.cbp;bb4win64.layout;packages/*")
set (CPACK_GENERATOR "ZIP")
set (CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};boxCore-package;ALL;/;${CMAKE_CURRENT_BINARY_DIR};Blackbox;ALL;/")
set (CPACK_PACKAGE_NAME "boxCore-package")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/Blackbox.ico")
set (CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/Blackbox.ico")
set (CPACK_NSIS_COMPRESSOR "/FINAL /SOLID lzma")
set (CPACK_PACKAGE_VERSION ${VERSION})
set (CPACK_PACKAGE_VENDOR "Carsomyr")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "Blackbox-${PACKAGE_ARCH}")
set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_ARCH}")
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY False)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
set (CPACK_NSIS_DISPLAY_NAME "Blackbox for Windows")

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)
add_subdirectory (coreplugins/bbKeys)
add_subdirectory (coreplugins/bbLeanBar)
add_subdirectory (coreplugins/boxBar)
endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)

set (CPACK_COMPONENT_GROUP_CORE_DISPLAY_NAME "Blackbox Core")
set (CPACK_COMPONENT_GROUP_CORE_EXPANDED True)
set (CPACK_COMPONENT_GROUP_CORE_DESCRIPTION
	"The main executable and config files")

set (CPACK_COMPONENT_GROUP_COREPLUGINS_DISPLAY_NAME "Core Plugins")
set (CPACK_COMPONENT_GROUP_COREPLUGINS_EXPANDED True)
set (CPACK_COMPONENT_GROUP_COREPLUGINS_DESCRIPTION
	"The main plugins intended for use with the shell")

include (CPack)
