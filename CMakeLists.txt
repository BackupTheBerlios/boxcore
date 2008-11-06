cmake_minimum_required(VERSION 2.6)

find_package(Doxygen)

set (CPACK_GENERATOR "ZIP")
set (CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};Blackbox;ALL;/")
set (CPACK_PACKAGE_NAME "Blackbox")
set (CPACK_PACKAGE_VERSION "1.16")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "blackbox")
set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
project ("Blackbox")

set (CORE_BUILD "yes")
add_subdirectory (blackbox)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)
add_subdirectory (coreplugins)
endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)

include (CPack)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY IMMEDIATE)
add_custom_target(doc ${DOXYGEN_EXECUTABLE})
