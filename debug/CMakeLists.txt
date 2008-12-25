cmake_minimum_required (VERSION 2.6)
project (DebugTools)

set (DEBUG_SRCS debug.cpp)

add_library(debuglib STATIC ${DEBUG_SRCS})
set_target_properties (debuglib PROPERTIES
	PREFIX ""
#	COMPILE_DEFINITIONS "UNICODE;_UNICODE"
)