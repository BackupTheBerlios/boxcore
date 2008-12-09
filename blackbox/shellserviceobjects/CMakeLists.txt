#cmake_minimum_required(VERSION 2.6)

#project (BLACKBOX)

set (SSO_SRCS
clsClsidSources.cpp
clsShellServiceObjects.cpp
)

add_library (shellserviceobjects STATIC ${SSO_SRCS})

if (MINGW)
set_target_properties (shellserviceobjects PROPERTIES
	LINK_FLAGS "-mwindows -s"
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
