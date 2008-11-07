cmake_minimum_required(VERSION 2.6)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

project ("Blackbox")

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH amd64)
set (PACKAGE_ARCH x64)
else (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH x86)
set (PACKAGE_ARCH x86)
endif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)

find_package(Doxygen)

set (CPACK_GENERATOR "ZIP;NSIS")
set (CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};Blackbox;ALL;/")
set (CPACK_PACKAGE_NAME "Blackbox")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/Blackbox.ico")
set (CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/Blackbox.ico")
set (CPACK_NSIS_COMPRESSOR "/FINAL /SOLID lzma")
set (CPACK_PACKAGE_VERSION "1.16")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "Blackbox-${PACKAGE_ARCH}")
set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_ARCH}")
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY False)

set (CPACK_COMPONENTS_ALL core configcore coreplugins)
set (CPACK_COMPONENT_CORE_DISPLAY_NAME "Blackbox core")
set (CPACK_COMPONENT_CORE_DESCRIPTION "The main blockbox executable and default configuration files")
set (CPACK_COMPONENT_CORE_REQUIRED True)
set (CPACK_COMPONENT_CONFIGCORE_DISPLAY_NAME "Blackbox config")
set (CPACK_COMPONENT_CONFIGCORE_DESCRIPTION "The config files for blackbox core")
set (CPACK_COMPONENT_COREPLUGINS_DISPLAY_NAME "Core plugins")
set (CPACK_COMPONENT_COREPLUGINS_DESCRIPTION "The bbKeys plugin for keyboard shortcuts and bbLeanbar to provide a system bar. Includes config files")

include (CPack)



set (CORE_BUILD "yes")
add_subdirectory (blackbox)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)
add_subdirectory (coreplugins)
endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)



configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
add_custom_target(doc ${DOXYGEN_EXECUTABLE})
