cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0008 NEW)
project (ShellServiceObjs)

set (SSO_SRCS
clsClsidSources.cpp
clsShellServiceObjects.cpp
)

add_library (shellserviceobjects STATIC ${SSO_SRCS})

if (MINGW)
set_target_properties (shellserviceobjects PROPERTIES
	LINK_FLAGS "-s"
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
