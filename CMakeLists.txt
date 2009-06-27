cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0008 NEW)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

project (boxCore)
set (BOXCORE_FULL_TREE 1)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)
set (VERSION_PATCH 1)
set (VERSION_BUILD 0)

set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

include(boxCore_options)

find_package(Doxygen)

if (DOXYGEN_EXECUTABLE)
    if (DOXYGEN_DOT_EXECUTABLE)
        set (USE_DOT "YES")
    else (DOXYGEN_DOT_EXECUTABLE)
        set (USE_DOT "NO")
    endif (DOXYGEN_DOT_EXECUTABLE)

    option (BOXCORE_DOCS_CHM "Generate a .chm file?" OFF)
    if (BOXCORE_DOCS_CHM)
        set (DOXYGEN_HHC_EXECUTABLE "hhc" CACHE FILEPATH "Path to the hhc program")
    else (BOXCORE_DOCS_CHM)
        set (DOXYGEN_HHC_EXECUTABLE "")
    endif (BOXCORE_DOCS_CHM)
    
    option (INTERNALDOCS "Generate internal project documentation?" OFF)
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
                  COMMAND ${ASTYLE_EXECUTABLE} --options=astyleconfig \"*.cpp\" \"*.h\"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
                  COMMENT "Formatting source files...")
endif (ASTYLE_EXECUTABLE)

add_subdirectory (debug)

set (CPACK_SOURCE_GENERATOR "ZIP")
set (CPACK_SOURCE_IGNORE_FILES ".hg/;build*;styles/*;config/*;.hgtags;.hgignore;bb4win64.cbp;bb4win64.layout;packages/*")
set (CPACK_PACKAGE_NAME "boxCore-package")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/boxCore.ico")
set (CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/blackbox/boxCore.ico")
set (CPACK_PACKAGE_VERSION ${VERSION})
set (CPACK_PACKAGE_VENDOR "Carsomyr")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "Blackbox-${PACKAGE_ARCH}")
set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_ARCH}")
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY False)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
set (CPACK_NSIS_DISPLAY_NAME "Blackbox for Windows")

include (CPack)

#cpack_configure_downloads(http://download.berlios.de/boxcore/
#                          UPLOAD_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Uploads
#                         )

add_subdirectory (blackbox)

install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/styles DESTINATION . COMPONENT Styles)

cpack_add_component(Styles
                    DISPLAY_NAME "Styles"
                    DESCRIPTION "A set of initial styles"
                    INSTALL_TYPES Standard Full
                   ) 

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)
add_subdirectory (coreplugins/bbKeys)
add_subdirectory (coreplugins/bbLeanBar)
add_subdirectory (coreplugins/boxBar)
endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/coreplugins/)

add_subdirectory (plugins/boxVolume)
add_subdirectory (plugins/bbInterface)

cpack_add_component_group(Core
                DISPLAY_NAME "Shell"
                DESCRIPTION "The main executable and config files"
                EXPANDED
               )
               
cpack_add_install_type(Standard
                       DISPLAY_NAME "Standard"
                      )

set (CPACK_COMPONENT_GROUP_COREPLUGINS_DISPLAY_NAME "Core Plugins")
set (CPACK_COMPONENT_GROUP_COREPLUGINS_EXPANDED True)
set (CPACK_COMPONENT_GROUP_COREPLUGINS_DESCRIPTION
	"The default plugins for the shell. Not installing all of these will give you an error message on first startup")
	
set (CPACK_COMPONENT_GROUP_PLUGINS_DISPLAY_NAME "Plugins")
set (CPACK_COMPONENT_GROUP_PLUGINS_EXPANDED True)
set (CPACK_COMPONENT_GROUP_PLUGINS_DESCRIPTION
    "Extra plugins that you can install")