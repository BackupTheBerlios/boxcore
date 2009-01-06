cmake_minimum_required (VERSION 2.6)
project (boxBar)

set (BOXBAR_SRCS
boxBar.cpp
clsBoxBar.cpp
clsItem.cpp
clsItemCollection.cpp

clsIconItem.cpp
clsTextItem.cpp

clsTaskItem.cpp
clsTaskItemCollection.cpp

clsLabelItem.cpp

clsWorkspaceLabel.cpp


clsTrayItem.cpp
clsTrayItemCollection.cpp



clsClockItem.cpp

clsFlexiSpacer.cpp

clsStyle.cpp

clsBar.cpp

clsTip.cpp

../utils/clsApiLoader.cpp
../../dynwinapi/clsUser32.cpp
../../dynwinapi/clsMsimg32.cpp
)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 4)

set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set (AUTHOR Carsomyr)

set (PLUGININFO_NAME ${PROJECT_NAME})
set (PLUGININFO_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set (PLUGININFO_AUTHOR ${AUTHOR})
set (PLUGININFO_URL "http://developer.berlios.de/projects/boxcore/")
set (PLUGININFO_EMAIL "carsomyr@users.berlios.de")

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/CPackConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake)

add_library(boxBar MODULE ${BOXBAR_SRCS})
include_directories(boxBar ../../blackbox ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(boxBar ${UNICOWSLIB} debuglib blackbox comctl32 gdi32)
set_target_properties (boxBar PROPERTIES
	PREFIX ""
	VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
)
if (DEBUG_CODE)
set_target_properties (boxBar PROPERTIES COMPILE_DEFINITIONS "UNICODE;_UNICODE;DEBUG")
else (DEBUG_CODE)
set_target_properties (boxBar PROPERTIES COMPILE_DEFINITIONS "UNICODE;_UNICODE;NDEBUG")
endif (DEBUG_CODE)
if (MINGW)
set_target_properties (boxBar PROPERTIES
	LINK_FLAGS "-s"
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions"
)
endif (MINGW)

install(TARGETS boxBar LIBRARY DESTINATION plugins/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/boxBar.rc DESTINATION plugins/${PROJECT_NAME} COMPONENT ${PROJECT_NAME}Config)

set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_CURRENT_BINARY_DIR};${PROJECT_NAME};ALL;/" PARENT_SCOPE)
