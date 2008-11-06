if (NOT DEFINED PLUGIN_BATCH)
cmake_minimum_required (VERSION 2.6)
project ("bbLeanbar")
endif (NOT DEFINED PLUGIN_BATCH)

set (BBLEANBAR_SRCS
bbLeanBar.cpp
../BBPlugin/BBPlugin.cpp
)

add_library(bbLeanBar SHARED ${BBLEANBAR_SRCS})
include_directories(bbLeanBar ../../blackbox)
target_link_libraries(bbLeanBar shell32 user32 kernel32 gdi32 comctl32 ole32 uuid blackbox)
set_target_properties (bbLeanBar PROPERTIES PREFIX "" 
	LINK_FLAGS "-mwindows -s" 
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)

install(TARGETS bbLeanBar RUNTIME DESTINATION plugins/bbLeanBar)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config/bbLeanBar.rc DESTINATION plugins/bbLeanBar)