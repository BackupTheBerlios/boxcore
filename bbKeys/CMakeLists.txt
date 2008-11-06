if (NOT DEFINED PLUGIN_BATCH)
cmake_minimum_required (VERSION 2.6)
project ("bbKeys")
endif (NOT DEFINED PLUGIN_BATCH)

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

install(TARGETS bbKeys RUNTIME DESTINATION plugins/bbKeys)
install(TARGETS winkeyhook RUNTIME DESTINATION plugins/bbKeys)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/bbKeys.rc DESTINATION plugins/bbKeys)