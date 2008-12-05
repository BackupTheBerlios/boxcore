cmake_minimum_required (VERSION 2.6)
project ("bbDeskMinimize")

set (BBDESKMINIMIZE_SRCS
bbDeskMinimize.cpp
)

add_library(bbDeskMinimize MODULE ${BBDESKMINIMIZE_SRCS})
include_directories(bbDeskMinimize  ../../blackbox)
target_link_libraries(bbDeskMinimize  shell32 user32 kernel32 gdi32 comctl32 blackbox)
set_target_properties (bbDeskMinimize  PROPERTIES PREFIX "")
if(MINGW)
set_target_properties (bbDeskMinimize  PROPERTIES
	LINK_FLAGS "-mwindows -s"
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)
endif(MINGW)
