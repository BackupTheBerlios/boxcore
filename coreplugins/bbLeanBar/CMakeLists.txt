cmake_minimum_required (VERSION 2.6)
project ("bbLeanBar")

set (BBLEANBAR_SRCS
bbLeanBar.cpp
../BBPlugin/BBPlugin.cpp
../utils/clsApiLoader.cpp
)

add_library(bbLeanBar MODULE ${BBLEANBAR_SRCS})
include_directories(bbLeanBar ../../blackbox)
target_link_libraries(bbLeanBar shell32 user32 kernel32 gdi32 comctl32 ole32 uuid blackbox)
set_target_properties (bbLeanBar PROPERTIES PREFIX "")

if (MINGW)
set_target_properties (bbLeanBar PROPERTIES
	LINK_FLAGS "-mwindows -s"
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)
endif (MINGW)

install(TARGETS bbLeanBar LIBRARY DESTINATION plugins/bbLeanBar COMPONENT bbLeanBar)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/bbLeanBar.rc DESTINATION plugins/bbLeanBar COMPONENT bbLeanBarConfig)

set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_CURRENT_BINARY_DIR};bbLeanBar;ALL;/" PARENT_SCOPE)
set (CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} bbLeanBar bbLeanBarConfig PARENT_SCOPE)
set (CPACK_COMPONENT_BBLEANBAR_DISPLAY_NAME "bbLeanBar" PARENT_SCOPE)
set (CPACK_COMPONENT_BBLEANBAR_DESCRIPTION
	"Plugin which creates a system bar" PARENT_SCOPE)
set (CPACK_COMPONENT_BBLEANBARCONFIG_DISPLAY_NAME "bbLeanBar default configuration" PARENT_SCOPE)
set (CPACK_COMPONENT_BBLEANBARCONFIG_DESCRIPTION
	"Enables commonly used items on the systembar and sets some options relating to apppearance of the bar" PARENT_SCOPE)

set (CPACK_COMPONENT_BBLEANBAR_GROUP "CorePlugins" PARENT_SCOPE)
set (CPACK_COMPONENT_BBLEANBAR_INSTALL_TYPES Full Upgrade PARENT_SCOPE)
set (CPACK_COMPONENT_BBLEANBARCONFIG_GROUP "CorePlugins" PARENT_SCOPE)
set (CPACK_COMPONENT_BBLEANBARCONFIG_INSTALL_TYPES Full PARENT_SCOPE)
