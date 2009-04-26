cmake_minimum_required (VERSION 2.6)
project (boxBar)
if (NOT CMAKE_MODULE_PATH)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeModules" CACHE STRING "Path to search for CMake modules")
endif (NOT CMAKE_MODULE_PATH)
include (boxCore_options OPTIONAL RESULT_VARIABLE BOXCORE_OPTIONS_FOUND)
if (BOXCORE_OPTIONS_FOUND STREQUAL "NOTFOUND")
message(FATAL_ERROR "Your CMAKE_MODULE_PATH is incorrect. Please set it to the location of boxCore_options.cmake and boxCore_plugins.cmake")
endif (BOXCORE_OPTIONS_FOUND STREQUAL "NOTFOUND")
if (NOT BOXCORE_FULL_TREE)
include (boxCore_plugins)
endif (NOT BOXCORE_FULL_TREE)
AllowUnicode()

set (BOXBAR_SRCS
clsItem.cpp
clsCollection.cpp
clsIcon.cpp
clsText.cpp

boxBar.cpp

clsTask.cpp
clsTaskArea.cpp

clsLabel.cpp

clsWorkspaceLabel.cpp


clsTrayIcon.cpp
clsTrayArea.cpp



clsClock.cpp

clsFlexiSpacer.cpp

clsStyle.cpp

clsBar.cpp

clsTip.cpp

clsButton.cpp

clsDropTarget.cpp

clsSettingsManager.cpp

../utils/clsApiLoader.cpp
../../dynwinapi/clsUser32.cpp
../../dynwinapi/clsMsimg32.cpp

../../utility/stringcopy.cpp

../utils/clsPlugin.cpp
../utils/monitor.cpp
)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)
set (VERSION_PATCH 0)
set (VERSION_SUFFIX RC3)

set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX}")

set (AUTHOR Carsomyr)

set (PLUGININFO_NAME ${PROJECT_NAME})
set (PLUGININFO_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX}")
set (PLUGININFO_AUTHOR ${AUTHOR})
set (PLUGININFO_URL "http://developer.berlios.de/projects/boxcore/")
set (PLUGININFO_EMAIL "carsomyr@users.berlios.de")

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/CPackConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake)

add_library(boxBar MODULE ${BOXBAR_SRCS})
include_directories(boxBar ${CMAKE_CURRENT_BINARY_DIR} ${BLACKBOX_INCLUDE_DIR})
target_link_libraries(boxBar ${UNICOWSLIB} ${BOXCORE_LIBRARY_DEBUG} blackbox comctl32 gdi32 ole32 uuid)
set_target_properties (boxBar PROPERTIES
	PREFIX ""
	VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX}"
)

if (MINGW)
set_target_properties (boxBar PROPERTIES
	LINK_FLAGS "${BOXCORE_LINK_FLAGS}"
	COMPILE_FLAGS "-Wall ${BOXCORE_COMPILE_FLAGS} -fno-exceptions"
)
endif (MINGW)

install(TARGETS boxBar LIBRARY DESTINATION plugins/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
if (UPDATE_PACKAGE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/boxBar.rc DESTINATION plugins/${PROJECT_NAME}/sampleconfig COMPONENT ${PROJECT_NAME}Config)
else (UPDATE_PACKAGE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/boxBar.rc DESTINATION plugins/${PROJECT_NAME} COMPONENT ${PROJECT_NAME}Config)
endif (UPDATE_PACKAGE)

set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_CURRENT_BINARY_DIR};${PROJECT_NAME};ALL;/" PARENT_SCOPE)
