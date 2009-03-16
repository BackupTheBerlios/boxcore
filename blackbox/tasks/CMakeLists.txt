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

if (BOXCORE_DEBUG)
set_target_properties (taskmanager PROPERTIES COMPILE_DEFINITIONS "DEBUG")
else (BOXCORE_DEBUG)
set_target_properties (taskmanager PROPERTIES COMPILE_DEFINITIONS "NDEBUG")
endif (BOXCORE_DEBUG)

if (MINGW)
set_target_properties (taskmanager PROPERTIES
	LINK_FLAGS "${BOXCORE_FLAGS_STRIP} ${BOXCORE_FLAGS_PROFILE} ${BOXCORE_FLAGS_DEBUG}"
	COMPILE_FLAGS "${BOXCORE_FLAGS_OPTIMIZE} ${BOXCORE_FLAGS_PROFILE} ${BOXCORE_FLAGS_DEBUG} -Wall -fno-rtti -fno-exceptions")
endif (MINGW)
