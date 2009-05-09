cmake_minimum_required (VERSION 2.6)
project ("boxVolume")

set (${PROJECT_NAME}_SRCS
boxVolume.cpp
)

add_library(boxVolume MODULE ${${PROJECT_NAME}_SRCS})
PluginOption(boxVolume OFF)
include_directories(boxVolume  ../../blackbox)
target_link_libraries(boxVolume  shell32 user32 kernel32 gdi32 comctl32 winmm ole32 blackbox)
set_target_properties (boxVolume  PROPERTIES PREFIX "")
if(MINGW)
set_target_properties (boxVolume  PROPERTIES
	LINK_FLAGS "-s"
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)
endif(MINGW)

InstallPlugin(boxVolume)
