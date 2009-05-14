cmake_minimum_required (VERSION 2.6)
project ("bbLeanBar")

set (BBLEANBAR_SRCS
bbLeanBar.cpp
../BBPlugin/BBPlugin.cpp
../utils/clsApiLoader.cpp
)

add_library(bbLeanBar MODULE ${BBLEANBAR_SRCS})
PluginOption(bbLeanBar ON)
include_directories(bbLeanBar ../../blackbox)
target_link_libraries(bbLeanBar shell32 user32 kernel32 gdi32 comctl32 ole32 uuid blackbox)
set_target_properties (bbLeanBar PROPERTIES PREFIX "")

if (MINGW)
set_target_properties (bbLeanBar PROPERTIES
	LINK_FLAGS "${BOXCORE_LINK_FLAGS}"
	COMPILE_FLAGS "${BOXCORE_COMPILE_FLAGS} -fno-rtti"
)
endif (MINGW)

set(bbLeanBar_DESCRIPTION "Plugin which creates a system bar")
set(STANDARD_PLUGIN 1)
InstallPlugin(bbLeanBar)