cmake_minimum_required(VERSION 2.6)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

project ("Blackbox")

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH amd64)
set (PACKAGE_ARCH x64)
set (WINAPI_DECORATOR "")
else (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH x86)
set (PACKAGE_ARCH x86)
set (WINAPI_DECORATOR "@8")
endif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)

find_package(Doxygen)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
add_custom_target(doc ${DOXYGEN_EXECUTABLE})

set (CPACK_ALL_INSTALL_TYPES Full Upgrade)

add_subdirectory (blackbox)

set (CPACK_GENERATOR "ZIP;NSIS")
set (CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};Blackbox;ALL;/")
set (CPACK_PACKAGE_NAME "Blackbox")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/Blackbox.ico")
set (CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/Blackbox.ico")
set (CPACK_NSIS_COMPRESSOR "/FINAL /SOLID lzma")
set (CPACK_PACKAGE_VERSION "1.16.1-nightly")
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