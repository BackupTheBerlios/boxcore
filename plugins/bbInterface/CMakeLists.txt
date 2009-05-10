cmake_minimum_required (VERSION 2.6)
project (bbInterface)
find_path(CMAKE_MODULE_PATH "boxCore_options.cmake" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeModules" "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules" DOC "Path to our custom CMake modules")
include (boxCore_options)
if (NOT BOXCORE_FULL_TREE)
include (boxCore_plugins)
endif (NOT BOXCORE_FULL_TREE)

set (BBINTERFACE_SRCS
AgentMaster.cpp
AgentType_Mixer.cpp
AgentType_SystemMonitor.cpp
ControlType_Button.cpp
ListMaster.cpp
SlitManager.cpp
AgentType_Bitmap.cpp
AgentType_Run.cpp
AgentType_TGA.cpp
ControlType_Label.cpp
MenuMaster.cpp
SnapWindow.cpp
AgentType_Broam.cpp
AgentType_StaticText.cpp
AgentType_Winamp.cpp
ControlType_Slider.cpp
MessageMaster.cpp
StyleMaster.cpp
AgentType_CompoundText.cpp
AgentType_SwitchedState.cpp
ConfigMaster.cpp
ModuleMaster.cpp
Tooltip.cpp
AgentType_Graph.cpp
AgentType_System.cpp
ControlMaster.cpp
DialogMaster.cpp
PluginMaster.cpp
WindowMaster.cpp
)

add_library(bbInterface MODULE ${BBINTERFACE_SRCS})
PluginOption(bbInterface OFF)

include_directories(bbInterface ${CMAKE_CURRENT_BINARY_DIR} ${BLACKBOX_INCLUDE_DIR})
target_link_libraries(bbInterface ${BOXCORE_LIBRARY_DEBUG} blackbox comctl32 gdi32 ole32 uuid winmm gdiplus msimg32)
set_target_properties (bbInterface PROPERTIES
	PREFIX ""
)

if (MINGW)
set_target_properties (bbInterface PROPERTIES
	LINK_FLAGS "${BOXCORE_LINK_FLAGS}"
	COMPILE_FLAGS "${BOXCORE_COMPILE_FLAGS} -fexceptions"
)
endif (MINGW)

InstallPlugin(bbInterface NO)