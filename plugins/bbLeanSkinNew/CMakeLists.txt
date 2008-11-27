set (BBLEANSKIN_SRCS
bbLeanSkinMod/bbLeanSkinMod.cpp
)

set (BBLEANSKIN_ENGINE_SRCS
bbLeanSkinEng/hookctl.cpp
bbLeanSkinEng/subclass.cpp
)

add_library(bbLeanSkinMod SHARED ${BBLEANSKIN_SRCS})
include_directories(bbLeanSkinMod  ../../blackbox)
target_link_libraries(bbLeanSkinMod  user32 kernel32 gdi32 blackbox)
set_target_properties (bbLeanSkinMod  PROPERTIES PREFIX "" 
	LINK_FLAGS "-s" 
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)

add_library(bbLeanSkinEng SHARED ${BBLEANSKIN_ENGINE_SRCS})
include_directories(bbLeanSkinEng  ../../blackbox)
target_link_libraries(bbLeanSkinEng  user32 kernel32 gdi32 blackbox)
set_target_properties (bbLeanSkinEng  PROPERTIES PREFIX "" 
	LINK_FLAGS "-s" 
	COMPILE_FLAGS "-Wall -Os -fno-rtti -fno-exceptions -fomit-frame-pointer"
)

install(TARGETS bbLeanSkinMod RUNTIME DESTINATION plugins/bbLeanSkin)
install(TARGETS bbLeanSkinEng RUNTIME DESTINATION plugins/bbLeanSkin)