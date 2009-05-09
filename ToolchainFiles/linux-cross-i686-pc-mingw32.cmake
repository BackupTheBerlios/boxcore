# this one is important
SET(CMAKE_SYSTEM_NAME Windows)
set (CMAKE_SYSTEM_PROCESSOR i686)

# specify the cross compiler
SET(CMAKE_C_COMPILER   i686-pc-mingw32-gcc)
set (CMAKE_CXX_COMPILER i686-mingw32-g++)
set (WINDRES i686-pc-mingw32-windres)
set (DLLTOOL i686-pc-mingw32-dlltool)