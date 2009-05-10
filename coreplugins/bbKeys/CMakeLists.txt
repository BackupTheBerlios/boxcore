cmake_minimum_required (VERSION 2.6)
project ("bbKeys")

set (BBKEYS_SRCS
BBKeys.cpp
)

set (WINKEYHOOK_SRCS
winkeyhook.cpp
)

add_library(bbKeys MODULE ${BBKEYS_SRCS})
PluginOption(bbKeys ON)
include_directories(bbKeys  ../../blackbox)
target_link_libraries(bbKeys  shell32 user32 kernel32 gdi32 comctl32 blackbox)
set_target_properties (bbKeys  PROPERTIES PREFIX "")

if (MINGW)
set_target_properties (bbKeys  PROPERTIES
	LINK_FLAGS "-mwindows -s"
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)
endif (MINGW)

add_library(winkeyhook MODULE ${WINKEYHOOK_SRCS})
include_directories(winkeyhook  ../../blackbox)
target_link_libraries(winkeyhook  shell32 user32 kernel32 gdi32 comctl32)
set_target_properties (winkeyhook  PROPERTIES PREFIX "")

if (MINGW)
set_target_properties (winkeyhook  PROPERTIES
	LINK_FLAGS "-mwindows -s"
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)
endif (MINGW)

set (bbKeys_DESCRIPTION "Plugin for defining and using hotkeys in Blackbox") 
set (STANDARD_PLUGIN 1)
InstallPlugin(bbKeys)
InstallPluginHelper(bbKeys winkeyhook)