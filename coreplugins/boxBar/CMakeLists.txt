cmake_minimum_required (VERSION 2.6)
project ("boxBar")

set (BOXBAR_SRCS
boxBar.cpp
clsBoxBar.cpp
clsItem.cpp
clsItemCollection.cpp

clsTaskItem.cpp
clsTaskItemCollection.cpp

clsLabelItem.cpp

clsIconItem.cpp
clsTextItem.cpp


clsTrayItem.cpp
clsTrayItemCollection.cpp



clsClockItem.cpp

clsFlexiSpacer.cpp

clsStyle.cpp

clsBar.cpp

../utils/clsApiLoader.cpp
)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)

set (PLUGININFO_NAME ${PROJECT_NAME})
set (PLUGININFO_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set (PLUGININFO_AUTHOR Carsomyr)
set (PLUGININFO_URL http://nonsense)
set (PLUGININFO_EMAIL "carsomyr@users.sourceforge.net")

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

add_library(boxBar MODULE ${BOXBAR_SRCS})
include_directories(boxBar ../../blackbox)
target_link_libraries(boxBar blackbox comctl32)
set_target_properties (boxBar PROPERTIES
	PREFIX ""
	LINK_FLAGS "-s -mwindows"
	COMPILE_FLAGS "-g -Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
	COMPILE_DEFINITIONS "UNICODE;_UNICODE"
	VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
)

install(TARGETS boxBar LIBRARY DESTINATION plugins/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/bbLeanBar.rc DESTINATION plugins/${PROJECT_NAME} COMPONENT ${PROJECT_NAME}Config)

set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_CURRENT_BINARY_DIR};${PROJECT_NAME};ALL;/" PARENT_SCOPE)
