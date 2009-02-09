cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0008 NEW)
project (VirtualDesktops)

set (VWM_SRCS
clsNullVWM.cpp
)

add_library (vwm STATIC ${VWM_SRCS})
target_link_libraries (vwm ${UNICOWSLIB} debuglib)

if (DEBUG_CODE)
set_target_properties (vwm PROPERTIES COMPILE_DEFINITIONS "DEBUG")
else (DEBUG_CODE)
set_target_properties (vwm PROPERTIES COMPILE_DEFINITIONS "NDEBUG")
endif (DEBUG_CODE)

if (MINGW)
set_target_properties (vwm PROPERTIES
	LINK_FLAGS "-s"
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
