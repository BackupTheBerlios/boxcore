if (NOT BOXCORE_OPTIONS_INCLUDED)
set (BOXCORE_OPTIONS_INCLUDED 1)
message (STATUS "Processing boxCore options")
option(BOXCORE_ENABLE_UNICOWS "Should we link against unicows.dll?" OFF)
option(BOXCORE_UNICODE "Compile unicode versions of the core and plugins" ON)
option(UPDATE_PACKAGE "Should we move configs to a sample folder" OFF)
option(BOXCORE_PROFILE "Compile with profiling support" OFF)
option(BOXCORE_STRIP "Strip executables and libraries" ON)
option (BOXCORE_DEBUG "Enable debug information" OFF)
option (BOXCORE_DEBUGMESSAGES "Activate code for generating debug messages" OFF)
set(BOXCORE_FLAGS_OPTIMIZE "-Os" CACHE STRING "Optimization flags")

if (BOXCORE_PROFILE)
set (BOXCORE_FLAGS_PROFILE "-pg")
else (BOXCORE_PROFILE)
set (BOXCORE_FLAGS_PROFILE "")
endif (BOXCORE_PROFILE)

if (BOXCORE_DEBUG)
set (BOXCORE_FLAGS_DEBUG "-g3")
else (BOXCORE_DEBUG)
set (BOXCORE_FLAGS_DEBUG "")
endif (BOXCORE_DEBUG)

if (BOXCORE_DEBUGMESSAGES)
set (BOXCORE_LIBRARY_DEBUG "debuglib")
add_definitions (-DDEBUG)
else (BOXCORE_DEBUGMESSAGES)
set (BOXCORE_LIBRARY_DEBUG "")
add_definitions (-DNDEBUG)
endif (BOXCORE_DEBUGMESSAGES)

if (BOXCORE_STRIP)
set (BOXCORE_FLAGS_STRIP "-s")
else (BOXCORE_STRIP)
set (BOXCORE_FLAGS_STRIP "")
endif (BOXCORE_STRIP)

if (BOXCORE_UNICODE)
set (PACKAGE_CHARTYPE "unicode")
function (AllowUnicode)
add_definitions (-DUNICODE -D_UNICODE)
endfunction (AllowUnicode)
else (BOXCORE_UNICODE)
set (PACKAGE_CHARTYPE "ansi")
function (AllowUnicode)
#Nothing neccesary if unicode not enabled
endfunction (AllowUnicode)
endif (BOXCORE_UNICODE)

function(PluginOption pluginName state)
option(PLUGIN_${pluginName} "Enable building the ${pluginName} plugin" ${state})

if (PLUGIN_${pluginName})
set_target_properties(${pluginName} PROPERTIES EXCLUDE_FROM_ALL false)
else (PLUGIN_${pluginName})
set_target_properties(${pluginName} PROPERTIES EXCLUDE_FROM_ALL true)
endif (PLUGIN_${pluginName})
endfunction(PluginOption pluginName state)

set (BOXCORE_COMPILE_FLAGS "${BOXCORE_FLAGS_PROFILE} ${BOXCORE_FLAGS_OPTIMIZE} ${BOXCORE_FLAGS_DEBUG} -DNOSERVICE -finput-charset=ISO-8859-1 -fexec-charset=ISO-8859-1 -fmessage-length=0 -Wall -fno-exceptions")
set (BOXCORE_LINK_FLAGS "${BOXCORE_FLAGS_STRIP} ${BOXCORE_FLAGS_PROFILE} ${BOXCORE_FLAGS_DEBUG} -fmessage-length=0 -fno-exceptions -static")

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH amd64)
set (PACKAGE_ARCH x64)
set (WINAPI_DECORATOR "")
set (BOXCORE_EXE_SUFFIX "64")
else (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH x86)
set (PACKAGE_ARCH x86)
set (WINAPI_DECORATOR "@8")
if (BOXCORE_UNICODE AND BOXCORE_ENABLE_UNICOWS)
add_library(unicows STATIC IMPORTED)
set_target_properties (unicows PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libunicows.a)
set (UNICOWSLIB "unicows")
else (BOXCORE_UNICODE AND BOXCORE_ENABLE_UNICOWS)
set (UNICOWSLIB "")
endif (BOXCORE_UNICODE AND BOXCORE_ENABLE_UNICOWS)
endif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)

endif (NOT BOXCORE_OPTIONS_INCLUDED)