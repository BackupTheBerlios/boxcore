if (NOT DEFINED CORE_BUILD)
cmake_minimum_required (VERSION 2.6)
project ("Blackbox core plugins")
endif (NOT DEFINED CORE_BUILD)

set (PLUGIN_BATCH "Yes")

add_subdirectory (bbLeanBar)
add_subdirectory (bbKeys)
#add_subdirectory (bbLeanSkinNew)
