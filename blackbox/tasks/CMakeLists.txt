cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0008 NEW)
project (TaskManagement)
AllowUnicode()

set (TASKMAN_SRCS
clsTask.cpp
clsLegacyTask.cpp
clsTaskManager.cpp
clsTaskManagerInterface.cpp

helpers.cpp
)

add_library (taskmanager STATIC ${TASKMAN_SRCS})
target_link_libraries (taskmanager ${UNICOWSLIB} debuglib)

if (MINGW)
set_target_properties (taskmanager PROPERTIES
	LINK_FLAGS "${BOXCORE_LINK_FLAGS}"
	COMPILE_FLAGS "${BOXCORE_COMPILE_FLAGS} -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
