cmake_minimum_required (VERSION 2.6)
project ("bbKeys")

set (BBKEYS_SRCS
BBKeys.cpp
)

set (WINKEYHOOK_SRCS
winkeyhook.cpp
)

add_library(bbKeys SHARED ${BBKEYS_SRCS})
include_directories(bbKeys  ../../blackbox)
target_link_libraries(bbKeys  shell32 user32 kernel32 gdi32 comctl32 blackbox)
set_target_properties (bbKeys  PROPERTIES PREFIX "" 
	LINK_FLAGS "-mwindows -s" 
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)

add_library(winkeyhook SHARED ${WINKEYHOOK_SRCS})
include_directories(winkeyhook  ../../blackbox)
target_link_libraries(winkeyhook  shell32 user32 kernel32 gdi32 comctl32 blackbox)
set_target_properties (winkeyhook  PROPERTIES PREFIX "" 
	LINK_FLAGS "-mwindows -s" 
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)

install(TARGETS bbKeys RUNTIME DESTINATION plugins/bbKeys COMPONENT bbKeys)
install(TARGETS winkeyhook RUNTIME DESTINATION plugins/bbKeys COMPONENT bbKeys)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/bbKeys.rc DESTINATION plugins/bbKeys COMPONENT bbKeysConfig)

set (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_CURRENT_BINARY_DIR};bbKeys;ALL;/" PARENT_SCOPE)
set (CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} bbKeys bbKeysConfig PARENT_SCOPE)
set (CPACK_COMPONENT_BBKEYS_DISPLAY_NAME "bbKeys" PARENT_SCOPE)
set (CPACK_COMPONENT_BBKEYS_DESCRIPTION
	"Plugin for defining and using hotkeys in Blackbox" PARENT_SCOPE)
set (CPACK_COMPONENT_BBKEYSCONFIG_DISPLAY_NAME "bbKeys default config" PARENT_SCOPE)
set (CPACK_COMPONENT_BBKEYSCONFIG_DESCRIPTION
	"Default hotkeys. Includes some to replicate explorer behaviour and some to access Blackbox functions" PARENT_SCOPE)

set (CPACK_COMPONENT_BBKEYS_GROUP "CorePlugins" PARENT_SCOPE)
set (CPACK_COMPONENT_BBKEYS_INSTALL_TYPES Full Upgrade PARENT_SCOPE)
set (CPACK_COMPONENT_BBKEYSCONFIG_GROUP "CorePlugins" PARENT_SCOPE)
set (CPACK_COMPONENT_BBKEYSCONFIG_INSTALL_TYPES Full PARENT_SCOPE)