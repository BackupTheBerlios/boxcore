set (BBLEANSKIN_SRCS
bbLeanSkin.cpp
)

set (BBLEANSKIN_ENGINE_SRCS
engine/hookctl.cpp
engine/subclass.cpp
)

add_library(bbLeanSkin SHARED ${BBLEANSKIN_SRCS})
include_directories(bbLeanSkin  ../../blackbox)
target_link_libraries(bbLeanSkin  user32 kernel32 gdi32 blackbox)
set_target_properties (bbLeanSkin  PROPERTIES PREFIX "" 
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

install(TARGETS bbLeanSkin RUNTIME DESTINATION plugins/bbLeanSkin)
install(TARGETS bbLeanSkinEng RUNTIME DESTINATION plugins/bbLeanSkin)