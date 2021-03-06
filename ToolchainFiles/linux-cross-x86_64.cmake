# this one is important
SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_SYSTEM_PROCESSOR "AMD64")

# specify the cross compiler
SET(CMAKE_C_COMPILER   x86_64-pc-mingw32-gcc)
SET(CMAKE_CXX_COMPILER   x86_64-pc-mingw32-g++)
set (WINDRES x86_64-pc-mingw32-windres)
set (DLLTOOL x86_64-pc-mingw32-dlltool)

# where is the target environment 
#SET(CMAKE_FIND_ROOT_PATH  /usr/x86_64-pc-mingw32)

# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
