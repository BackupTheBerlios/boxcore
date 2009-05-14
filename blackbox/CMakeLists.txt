project (Blackbox)
cmake_minimum_required(VERSION 2.6)
cmake_policy (SET CMP0008 NEW)

if (NOT WINDRES)
set (WINDRES windres.exe)
endif (NOT WINDRES)

set (BLACKBOX_CORE_SRCS
Blackbox.cpp
BBApi.cpp
Utils.cpp
BImage.cpp
Settings.cpp
PluginManager.cpp
Desk.cpp
Toolbar.cpp
DesktopMenu.cpp
ConfigMenu.cpp
m_alloc.cpp
Tinylist.cpp
Install.cpp
BBRoot.cpp
Pidl.cpp
VolumeControl.cpp
)

set (BLACKBOX_NEW_SRCS
clsSystemTrayIcon.cpp
clsSystemInfo.cpp
clsMessageManager.cpp
clsBBTask.cpp
callbacks.cpp

../utility/stringcopy.cpp
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

set (BLACKBOX_CONFIG
config/blackbox.rc
config/extensions.rc
config/menu.rc
config/menubb.rc
config/plugins.rc
config/shellfolders.rc
config/stickywindows.ini
)

set (SHELLSERVICES_SRCS
shellservices/clsShellServiceWindow.cpp
shellservices/clsNotifyIconHandler.cpp
shellservices/clsNotificationIcon.cpp

shellservices/hash.cpp

../dynwinapi/clsShlwapi.cpp
../dynwinapi/clsUser32.cpp

shellservices/clsService.cpp
shellservices/clsServiceManager.cpp
shellservices/clsServiceRegistrar.cpp
shellservices/clsShellTrayWndSrv.cpp
shellservices/clsAppbarSrv.cpp
shellservices/clsNotifyIconRectSrv.cpp
shellservices/clsNotifyIconSrv.cpp
shellservices/clsStartupSrv.cpp
shellservices/clsShellServiceObjectsSrv.cpp
)

set (BLACKBOX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)
set (VERSION_PATCH 1)
set (VERSION_BUILD 0)

set (PLUGININFO_NAME ${PROJECT_NAME})
set (VERSION "(boxCore ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})")
set (PLUGININFO_AUTHOR Carsomyr)
set (PLUGININFO_URL http://developer.berlios.de/projects/boxcore/)
set (PLUGININFO_EMAIL "carsomyr@users.berlios.de")

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
${CMAKE_CURRENT_BINARY_DIR}/version.h)

if(MINGW)
  # resource compilation for mingw
  set (RC_MANIFEST_LINE "APP_MANIFEST RT_MANIFEST blackbox.exe.manifest")
  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resource.o
                     COMMAND ${WINDRES} -I${CMAKE_CURRENT_SOURCE_DIR}
                                         -i${CMAKE_CURRENT_BINARY_DIR}/resource.rc
                                         -o ${CMAKE_CURRENT_BINARY_DIR}/resource.o)
  SET(BLACKBOX_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/resource.o)
else(MINGW)
  SET(BLACKBOX_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/resource.rc)
endif(MINGW)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource.rc.in
${CMAKE_CURRENT_BINARY_DIR}/resource.rc)

add_executable (blackbox WIN32 ${BLACKBOX_CORE_SRCS} ${BLACKBOX_MENU_SRCS} ${BLACKBOX_NEW_SRCS} ${BLACKBOX_RESOURCES} ${SHELLSERVICES_SRCS} )
include_directories(blackbox ${CMAKE_CURRENT_BINARY_DIR})
#add_custom_command (TARGET blackbox POST_BUILD
#	COMMAND ${DLLTOOL} -d ${CMAKE_CURRENT_BINARY_DIR}/libBlackbox.def -l ${CMAKE_CURRENT_BINARY_DIR}/libblackbox.a -D blackbox.exe
#	COMMENT "Building proper library")
target_link_libraries (blackbox ${UNICOWSLIB}
	debuglib shellserviceobjects taskmanager vwm shlwapi version ole32 uuid comctl32 comdlg32 shell32 advapi32 winmm gdi32)
set_target_properties (blackbox PROPERTIES
	IMPORT_SUFFIX .a
	ENABLE_EXPORTS true
#	OUTPUT_NAME "blackbox${BOXCORE_EXE_SUFFIX}"
)

if (MINGW)
set_target_properties (blackbox PROPERTIES
	LINK_FLAGS "${BOXCORE_LINK_FLAGS} ${CMAKE_CURRENT_BINARY_DIR}/libBlackbox.def"
	COMPILE_FLAGS "${BOXCORE_COMPILE_FLAGS} -Wall -fno-exceptions -fmessage-length=0"
)
endif (MINGW)

add_library(TrayForwarder SHARED hooks/TrayForwarder.cpp)
set_target_properties (TrayForwarder  PROPERTIES PREFIX "")

if (MINGW)
set_target_properties (TrayForwarder PROPERTIES
	LINK_FLAGS "${BOXCORE_FLAGS_STRIP}"
	COMPILE_FLAGS "${BOXCORE_FLAGS_OPTIMIZE} -Wall -fno-rtti -fno-exceptions -fmessage-length=0"
)
endif (MINGW)

if (MINGW)
set (DEF_HEADER "")
else (MINGW)
set (DEF_HEADER "LIBRARY Blackbox${BOXCORE_EXE_SUFFIX}.exe")
endif (MINGW)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libBlackbox.def.in
${CMAKE_CURRENT_BINARY_DIR}/libBlackbox.def)

add_subdirectory (shellserviceobjects)
#add_subdirectory (shellservices)
add_subdirectory (tasks)
add_subdirectory (vwm)

install(TARGETS blackbox RUNTIME DESTINATION . COMPONENT Blackbox)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/${BLACKBOX_CONFIG} DESTINATION . COMPONENT BlackboxConfig)

#set (CPACK_COMPONENTS_ALL Blackbox BlackboxConfig PARENT_SCOPE)
cpack_add_component(Blackbox
                    DISPLAY_NAME "Blackbox"
                    DESCRIPTION "The main blockbox executable"
                    REQUIRED
                    GROUP Core
                    INSTALL_TYPES Standard Full StandardUpdate FullUpdate
                   ) 

cpack_add_component(BlackboxConfig
                    DISPLAY_NAME "Blackbox default configuration"
                    DESCRIPTION "The config files for blackbox core"
                    GROUP "Core"
                    DEPENDS Blackbox
                    INSTALL_TYPES Standard Full
                   )