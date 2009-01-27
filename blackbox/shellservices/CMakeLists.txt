cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0008 NEW)
project (ShellServices)

set (SYSTEMTRAY_SRCS
clsShellServiceWindow.cpp
clsNotifyIconHandler.cpp
clsNotificationIcon.cpp
clsAppbarHandler.cpp

hash.cpp

../../dynwinapi/clsShlwapi.cpp
../../dynwinapi/clsUser32.cpp
)

add_library (shellservices STATIC ${SYSTEMTRAY_SRCS})
target_link_libraries (shellservices ${UNICOWSLIB} debuglib shlwapi)

if (DEBUG_CODE)
set_target_properties (shellservices PROPERTIES COMPILE_DEFINITIONS "DEBUG")
else (DEBUG_CODE)
set_target_properties (shellservices PROPERTIES COMPILE_DEFINITIONS "NDEBUG")
endif (DEBUG_CODE)

if (MINGW)
set_target_properties (shellservices PROPERTIES
	LINK_FLAGS "-s"
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
