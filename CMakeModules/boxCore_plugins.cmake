find_path(BLACKBOX_INCLUDE_DIR "BBApi.h" PATHS "." "../../blackbox" DOC "Location of BBApi.h")
message (STATUS "Found BBApi.h in ${BLACKBOX_INCLUDE_DIR}")

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set(EXTRA_BUILD_PATH "../../build64/blackbox")
else (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)
set(EXTRA_BUILD_PATH "../../build32/blackbox")
endif (${CMAKE_SYSTEM_PROCESSOR} STREQUAL AMD64)

find_library(BLACKBOX_LIBRARY "blackbox" PATHS "." "${CMAKE_CURRENT_BINARY_DIR}" "../../blackbox" "../../build/blackbox" "${EXTRA_BUILD_PATH}" DOC "Location of import library for blackbox.exe")
add_library(blackbox SHARED IMPORTED)
set_target_properties(blackbox PROPERTIES
    IMPORTED_IMPLIB "${BLACKBOX_LIBRARY}"
    IMPORTED_LOCATION "${BLACKBOX_LIBRARY}"   
)
message (STATUS "Found Blackbox import library in ${BLACKBOX_LIBRARY}")
if (BLACKBOX_INCLUDE_DIR STREQUAL "BLACKBOX_INCLUDE_DIR-NOTFOUND" OR BLACKBOX_LIBRARY STREQUAL "BLACKBOX_LIBRARY-NOTFOUND")
message (FATAL_ERROR "Either the inlude file or link library could not be found. Plese specifiy them manually")
endif (BLACKBOX_INCLUDE_DIR STREQUAL "BLACKBOX_INCLUDE_DIR-NOTFOUND" OR BLACKBOX_LIBRARY STREQUAL "BLACKBOX_LIBRARY-NOTFOUND")

if (BOXCORE_DEBUG)
message (FATAL_ERROR "Standalone debug builds are not currently supported")
endif (BOXCORE_DEBUG)  