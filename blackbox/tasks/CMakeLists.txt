cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0008 NEW)
project (TaskManagement)

set (TASKMAN_SRCS
clsTask.cpp
clsLegacyTask.cpp
clsTaskManager.cpp
clsTaskManagerInterface.cpp

helpers.cpp
)

add_library (taskmanager STATIC ${TASKMAN_SRCS})
target_link_libraries (taskmanager ${UNICOWSLIB} debuglib)

if (DEBUG_CODE)
set_target_properties (taskmanager PROPERTIES COMPILE_DEFINITIONS "DEBUG")
else (DEBUG_CODE)
set_target_properties (taskmanager PROPERTIES COMPILE_DEFINITIONS "NDEBUG")
endif (DEBUG_CODE)

if (MINGW)
set_target_properties (taskmanager PROPERTIES
	LINK_FLAGS "-s"
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
