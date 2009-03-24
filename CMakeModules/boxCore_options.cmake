if (NOT BOXCORE_OPTIONS_INCLUDED)
set (BOXCORE_OPTIONS_INCLUDED 1)
message (STATUS "Processing boxCore options")
option(USE_UNICOWS "Should we link against unicows.dll?" ON)
option(BOXCORE_UNICODE "Compile unicode versions of the core and plugins" ON)
option(UPDATE_PACKAGE "Should we move configs to a sample folder" OFF)
option(BOXCORE_PROFILE "Compile with profiling support" OFF)
option(BOXCORE_STRIP "Strip executables and libraries" ON)
option (BOXCORE_DEBUG "Activate code for generating debug messages and enable debug information" OFF)
set(BOXCORE_FLAGS_OPTIMIZE "-Os" CACHE STRING "Optimization flags")

if (BOXCORE_PROFILE)
set (BOXCORE_FLAGS_PROFILE "-pg")
else (BOXCORE_PROFILE)
set (BOXCORE_FLAGS_PROFILE "")
endif (BOXCORE_PROFILE)

if (BOXCORE_DEBUG)
set (BOXCORE_FLAGS_DEBUG "-gstabs")
set (BOXCORE_LIBRARY_DEBUG "debuglib")
add_definitions (-DDEBUG)
else (BOXCORE_DEBUG)
set (BOXCORE_FLAGS_DEBUG "")
set (BOXCORE_LIBRARY_DEBUG "")
add_definitions (-DNDEBUG)
endif (BOXCORE_DEBUG)

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

set (BOXCORE_COMPILE_FLAGS "${BOXCORE_FLAGS_PROFILE} ${BOXCORE_FLAGS_OPTIMIZE} ${BOXCORE_FLAGS_DEBUG}")
set (BOXCORE_LINK_FLAGS "${BOXCORE_FLAGS_STRIP} ${BOXCORE_FLAGS_PROFILE} ${BOXCORE_FLAGS_DEBUG}")

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH amd64)
set (PACKAGE_ARCH x64)
set (WINAPI_DECORATOR "")
#set (BOXCORE_EXE_SUFFIX "64")
else (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set (ARCH x86)
set (PACKAGE_ARCH x86)
set (WINAPI_DECORATOR "@8")
if (USE_UNICOWS)
add_library(unicows STATIC IMPORTED)
set_target_properties (unicows PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libunicows.a)
set (UNICOWSLIB "unicows")
else (USE_UNICOWS)
set (UNICOWSLIB "")
endif (USE_UNICOWS)
endif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)

endif (NOT BOXCORE_OPTIONS_INCLUDED)