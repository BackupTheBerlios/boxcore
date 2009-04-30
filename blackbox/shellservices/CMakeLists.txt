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

clsService.cpp
clsServiceManager.cpp
clsShellTrayWndSrv.cpp
clsAppbarSrv.cpp
clsNotifyIconRectSrv.cpp
clsNotifyIconSrv.cpp
clsStartupSrv.cpp
)

add_library (shellservices STATIC ${SYSTEMTRAY_SRCS})
target_link_libraries (shellservices ${UNICOWSLIB} debuglib shlwapi)

if (MINGW)
set_target_properties (shellservices PROPERTIES
	LINK_FLAGS "${BOXCORE_LINK_FLAGS}"
	COMPILE_FLAGS "${BOXCORE_COMPILE_FLAGS} -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
