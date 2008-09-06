project (BLACKBOX)

set (BLACKBOX_CORE_SRCS
Blackbox.cpp
BBApi.cpp
Utils.cpp
BImage.cpp
Settings.cpp
PluginManager.cpp
MessageManager.cpp
Workspaces.cpp
Tray.cpp
Desk.cpp
Toolbar.cpp
DesktopMenu.cpp
ConfigMenu.cpp
m_alloc.cpp
Tinylist.cpp
Install.cpp
BBRoot.cpp
BBVWM.cpp
Pidl.cpp
VolumeControl.cpp
)

set (BLACKBOX_MENU_SRCS
Menu/MenuMaker.cpp
Menu/MenuItem.cpp
Menu/Menu.cpp
Menu/TitleItem.cpp
Menu/FolderItem.cpp
Menu/CommandItem.cpp
Menu/SpecialFolder.cpp
Menu/Contextmenu.cpp
Menu/Dragsource.cpp
Menu/Droptarget.cpp
)

if(MINGW)
  # resource compilation for mingw
  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resource.o
                     COMMAND ${WINDRES} -I${CMAKE_CURRENT_SOURCE_DIR}
                                         -i${CMAKE_CURRENT_SOURCE_DIR}/resource.rc
                                         -o ${CMAKE_CURRENT_BINARY_DIR}/resource.o)
  SET(BLACKBOX_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/resource.o)
else(MINGW)
  SET(BLACKBOX_RESOURCES resource.rc)
endif(MINGW)

add_executable (blackbox ${BLACKBOX_CORE_SRCS} ${BLACKBOX_MENU_SRCS} ${BLACKBOX_RESOURCES})
add_custom_command (TARGET blackbox POST_BUILD
	COMMAND ${DLLTOOL} -d ${CMAKE_CURRENT_SOURCE_DIR}/libBlackbox.def -l ${CMAKE_CURRENT_BINARY_DIR}/libblackbox.a -D blackbox.exe
	COMMENT "Building proper library")
target_link_libraries (blackbox version ole32 uuid comctl32 comdlg32 shell32 advapi32 winmm gdi32)
set_target_properties (blackbox PROPERTIES 
	IMPORT_SUFFIX .a 
	LINK_FLAGS "-mwindows -s -Wl,--output-def,/home/shaun/test.def" 
	COMPILE_FLAGS "-Os -Wall -fno-rtti -fno-exceptions"
	ENABLE_EXPORTS true)

install(TARGETS blackbox RUNTIME DESTINATION .)