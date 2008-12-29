cmake_minimum_required(VERSION 2.6)

project (ShellServices)

set (SYSTEMTRAY_SRCS
clsSystemTray.cpp
clsShellServiceWindow.cpp
clsNotifyIconHandler.cpp
clsNotificationIcon.cpp
clsTrayItem.cpp

../../dynwinapi/clsShlwapi.cpp
../../dynwinapi/clsUser32.cpp
)

add_library (systemtray STATIC ${SYSTEMTRAY_SRCS})
target_link_libraries (systemtray debuglib shlwapi)

if (MINGW)
set_target_properties (systemtray PROPERTIES
	LINK_FLAGS "-mwindows -s"
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
