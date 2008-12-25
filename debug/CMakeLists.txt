cmake_minimum_required (VERSION 2.6)
project (DebugTools)

set (DEBUG_SRCS debug.cpp)

add_library(debug STATIC ${DEBUG_SRCS})
set_target_properties (debug PROPERTIES
	PREFIX ""
#	COMPILE_DEFINITIONS "UNICODE;_UNICODE"
)