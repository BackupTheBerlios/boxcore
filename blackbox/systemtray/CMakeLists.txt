#cmake_minimum_required(VERSION 2.6)

#project (BLACKBOX)

set (SYSTEMTRAY_SRCS
clsSystemTray.cpp
clsTrayItem.cpp
)

add_library (systemtray STATIC ${SYSTEMTRAY_SRCS})

set_target_properties (systemtray PROPERTIES
	LINK_FLAGS "-mwindows -s"
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions")
