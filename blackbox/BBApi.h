/** @file BBApi.h
  * @brief Contains the defintions for use by plugins
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2001-2003 The Blackbox for Windows Development Team -->
  * <!-- Copyright © 2004-2007 grischka -->
  * <!-- Copyright © 2008 Carsomyr -->
  * Copyright &copy; 2001-2003 The Blackbox for Windows Development Team @n
  * Copyright &copy; 2004-2007 grischka @n
  * Copyright &copy; 2008 Carsomyr
  * @par Links:
  * http://developer.berlios.de/projects/boxcore @n
  * http://bb4win.sourceforge.net/bblean @n
  * http://sourceforge.net/projects/bb4win @n
  * @par License:
  * boxCore, bbLean and bb4win are free software, released under the GNU General
  * Public License (GPL version 2 or later), with an extension that allows
  * linking of proprietary modules under a controlled interface. This means
  * that plugins etc. are allowed to be released under any license the author
  * wishes. For details see:
  * @par
  * http://www.fsf.org/licenses/gpl.html @n
  * http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface
  * @par
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  */

#ifndef __BBAPI_H_
#define __BBAPI_H_

/*------------------------------------------ */
/* compiler specifics */
/*------------------------------------------ */

#ifdef __GNUC__
#define _WIN32_IE 0x0501
#endif

#ifdef __BORLANDC__
#define DLL_EXPORT /* .def file required to work around underscores */
#define _RWSTD_NO_EXCEPTIONS 1
#endif

#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif

/*------------------------------------------ */
/* windows include */
/*------------------------------------------ */

#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0502
#include <windows.h>
#include <stdio.h>

#define ISNULL = NULL
/** @defgroup bbapi Plugin API
  * @brief The plugin API is divided into several main groupings. This page attempts to make it
  * easy to find a function you that does what you want. If you just want a flat list of the api contents
  * see BBApi.h
  *
  * @{
  */

/** @defgroup bbapi_basic Basic API
  * @brief This part documents the part of the api that 99% of plugins will use. These functions and window
  * messages are essential to communication with Blackbox
  */

/** @defgroup bbapi_tasks Tasks API
  * @brief This section of the API contains the functions, window message and supporting information
  * you need to interface to the tasks functionality in Blackbox
  *
  * This part of the API is generally used by system bars. Any other plugin that deals with organising, displaying
  * or otherwise interacting with the running applications will probably also use at least some of these functions.
  */

/** @defgroup bbapi_tray System Tray API
  * @brief This section of the API contains the functions, window message and supporting information
  * you need to interface to the system tray functionality in Blackbox.
  *
  * This part of the API is generally used by system bars or standalone system tray plugins
  */

/** @defgroup bbapi_bblean bbLean only API
  * @brief These functions are only present in bbLean and derivatives. Alternatives are suggested where possible.
  */

/** @brief Maximum buffer size to use
  *
  * This define was used for the sizes of buffers. It is better
  * to avoid this and size your buffer appropriately because the value for this may
  * differ between branches or forks. Use MAX_PATH instead
  * for paths, and choose a reasonable size for other buffers.
  * @note The bbLean core (and derivatives) use 1024 for this internally
  */
#define MAX_LINE_LENGTH 4096

/** @name Gradients
  * Gradient type definitions. It is unlikely that a plugin needs these
  */

///@{
/** @brief A gradient from the left edge to the right
  */
#define B_HORIZONTAL 0
/** @brief A gradient from the top edge to the bottom
  */
#define B_VERTICAL   1
/** @brief A gradient from the top left corner to the bottom right corner
  */
#define B_DIAGONAL   2
/** @brief A gradient from the top right corner to the bottom left corner
  */
#define B_CROSSDIAGONAL 3
/** @brief Gradient that starts at the corners and changes towards the center of the rectangle and sides
  */
#define B_PIPECROSS  4
/** @brief Gradient that starts in the center and changes towards the edges in an elliptic shape
  */
#define B_ELLIPTIC   5
/** @brief Gradient that starts in the center and changes towards the edges in a rectangular
  */
#define B_RECTANGLE  6
/** @brief Gradient that starts at the corners and changes towards the center of the rectangle
  */
#define B_PYRAMID    7
/** @brief A single solid color
  */
#define B_SOLID      8
#define B_SPLITVERTICAL 9
#define B_MIRRORHORIZONTAL 10
#define B_MIRRORVERTICAL 11

/* Bevels */
#define BEVEL_FLAT   0
#define BEVEL_RAISED 1
#define BEVEL_SUNKEN 2
#define BEVEL1 1
#define BEVEL2 2

/// @}

/*=========================================================================== */
/* Blackbox messages */

/** @brief Window Message: Select window messages to recieve
  *
  * This is the first message that a plugin sends to the core to indicate which messages
  * it is interested in. The core will never send this message to a plugin.
  *
  * This message is usually sent during the handling of the WM_CREATE message by the plugin window.
  * @par wParam:
  * Set to the window handle of the plugin.
  * @par lParam:
  * Set to an int* which points to an array of window message numbers to recieve. The last element in this array should
  * be 0 to mark the end
  * @par Example:
  * @code
  * static int msgs = {BB_BROADCAST, BB_RECONFIGURE, 0};
  * PostMessage(GetBBWnd(), BB_REGISTERMESSAGE, (WPARAM)hWnd, (LPARAM)msgs);
  * @endcode
  * @ingroup bbapi_basic
  */
#define BB_REGISTERMESSAGE      10001

/** @brief Window Message: Cancel recieving window messages
  *
  * This should be the last message a plugin sends to the core before begin unloaded. Its usage is the same as
  * BB_REGISTERMESSAGE, except that this message cancels the core sending message. The core will
  * never send this message to a plugin.
  *
  * This message is usually sent during the handling of the WM_DESTROY message by the plugin window.
  * @par wParam:
  * Set to the window handle of the plugin.
  * @par lParam:
  * Set to an int* which points to an array of window message numbers to recieve. The last element in this array should
  * be 0 to mark the end
  * @ingroup bbapi_basic
  */
#define BB_UNREGISTERMESSAGE    10002

/* ----------------------------------- */
/** @brief The core must quit
  */
#define BB_QUIT                 10101
#define BB_RESTART              10102
#define BB_RECONFIGURE          10103
#define BB_SETSTYLE             10104 /* lParam: const char* stylefile */
#define BB_EXITTYPE             10105 /* lParam: 0=Shutdown/Reboot/Logoff 1=Quit 2=Restart */
#define B_SHUTDOWN 0
#define B_QUIT 1
#define B_RESTART 2
#define BB_TOOLBARUPDATE        10106
#define BB_SETTHEME             10107 /* xoblite */

/* ----------------------------------- */
#define BB_EDITFILE             10201
/* wParam: 0=CurrentStyle, 1=menu.rc, 2=plugins.rc 3=extensions.rc 4=blackbox.rc
  -1=filename in (LPCSTR)lParam */

#define BB_EXECUTE              10202 /* Send a command or broam for execution (in lParam) */
#define BB_ABOUTSTYLE           10203
#define BB_ABOUTPLUGINS         10204

/* ----------------------------------- */
#define BB_MENU                 10301
/* wParam: 0=Main menu, 1=Workspaces menu, 2=Toolbar menu */
#define BB_MENU_ROOT        0
#define BB_MENU_TASKS       1
#define BB_MENU_TOOLBAR     2
#define BB_MENU_ICONS       3
#define BB_MENU_PLUGIN      4
#define BB_MENU_CONTEXT     5
#define BB_MENU_BYBROAM     6
#define BB_MENU_BYBROAM_KBD 7

#define BB_HIDEMENU             10302
#define BB_TOGGLETRAY           10303   /* toggls system bar's etc. */
#define BB_TOGGLESYSTEMBAR      10303   /* xoblite */
#define BB_SETTOOLBARLABEL      10304   /* Set the toolbar label to (LPCSTR)lParam (returns to normal after 2 seconds) */
#define BB_TOGGLEPLUGINS        10305
#define BB_SUBMENU              10306
#define BB_TOGGLESLIT           10307   /* xoblite */
#define BB_TOGGLETOOLBAR        10308   /* xoblite */

/* ----------------------------------- */
#define BB_SHUTDOWN             10401   /* wParam: 0=Shutdown, 1=Reboot, 2=Logoff, 3=Hibernate, 4=Suspend, 5=LockWorkstation */
#define BB_RUN                  10402

/* ----------------------------------- */
#define BB_DESKTOPINFO          10501
#define BB_LISTDESKTOPS         10502
#define BB_SWITCHTON            10503
#define BB_BRINGTOFRONT         10504   /* lParam: Window to activate */
/* wParam flag: Zoom window into current workspace and activate: */
#define BBBTF_CURRENT 4

/* ----------------------------------- */
#define BB_WORKSPACE            10505
/* wParam values: */
#define BBWS_DESKLEFT         0
#define BBWS_DESKRIGHT        1
#define BBWS_ADDDESKTOP       2
#define BBWS_DELDESKTOP       3
#define BBWS_SWITCHTODESK     4  /* lParam: workspace to switch to. */
#define BBWS_GATHERWINDOWS    5
#define BBWS_MOVEWINDOWLEFT   6  /* lParam: hwnd or NULL for foregroundwindow */
#define BBWS_MOVEWINDOWRIGHT  7  /* lParam: hwnd or NULL for foregroundwindow */
#define BBWS_PREVWINDOW       8  /* lParam: 0=current / 1=all workspaces */
#define BBWS_NEXTWINDOW       9  /* lParam: 0=current / 1=all workspaces */
#define BBWS_LASTDESK         10

#define BBWS_TOGGLESTICKY     12 /* lParam: hwnd or NULL for foregroundwindow */
#define BBWS_EDITNAME         13
#define BBWS_MAKESTICKY       14 /* lParam: hwnd or NULL for foregroundwindow */
#define BBWS_CLEARSTICKY      15 /* lParam: hwnd or NULL for foregroundwindow */

#define BBWS_MINIMIZEALL      20
#define BBWS_RESTOREALL       21
#define BBWS_TILEVERTICAL     22
#define BBWS_TILEHORIZONTAL   23
#define BBWS_CASCADE          24

#define BBWS_MOVEWINDOWTOWS   25  /* lParam: desktop number, bool follow */

/*------------------------------------------ */


/** @brief Window Message: The task information in the core has been updated
  *
  * This message is sent by the core when a task is added, deleted or otherwise modified.
  * Plugins should never send this message, only recieve it.
  * @par wParam value:
  * An HWND indicating the affected task, except when the lParam is  ::TASKITEM_REFRESH where it is NULL.
  * @par lParam value:
  * Possible lParams are ::TASKITEM_ADDED, ::TASKITEM_MODIFIED, ::TASKITEM_ACTIVATED,
  * ::TASKITEM_REMOVED, ::TASKITEM_REFRESH or ::TASKITEM_FLASHED.
  * See individual items for information on why they are set
  * @ingroup bbapi_tasks
  */
#define BB_TASKSUPDATE          10506

/** @brief lParam: A new task was created
  *
  * wParam is the HWND of the new task
  * @ingroup bbapi_tasks
  */
#define TASKITEM_ADDED 0

/** @brief lParam: An existing task was modified
  *
  * wParam is the HWND of the modified task
  * @ingroup bbapi_tasks
  */
#define TASKITEM_MODIFIED 1

/** @brief lParam: The active task has changed
  *
  * wParam is the HWND of the new active task
  * @ingroup bbapi_tasks
  */
#define TASKITEM_ACTIVATED 2

/** @brief lParam: A task has been deleted
  *
  * wParam is the HWND that the task had
  * @ingroup bbapi_tasks
  */
#define TASKITEM_REMOVED 3

/** @brief lParam: An existing task has changed workspaces
  *
  * wParam is NULL
  * @ingroup bbapi_tasks
  */
#define TASKITEM_REFRESH 4

/** @brief lParam: An existing task is trying to get attention
  *
  * wParam is the HWND of the task whose icon should be flashed
  * @ingroup bbapi_tasks
  */
#define TASKITEM_FLASHED 5

/** @brief Window Message: A tray icon has been updated
  *
  * This message is sent by the core when there is a new tray icon, an existing tray icon has changed
  * or an existing tray icon has been removed. Plugins should not send this message.
  * @par wParam:
  * The value of the wParam is not consistent between branches, do not use it
  * @par lParam:
  * The possible values for the lParam are ::TRAYICON_ADDED, ::TRAYICON_MODIFIED or ::TRAYICON_REMOVED.
  * @ingroup bbapi_tray
  */
#define BB_TRAYUPDATE           10507

/** @brief Window Message: The tray must be cleaned
  *
  * This message is sent to the core by plugins if they find a tray icon
  * whose owner no longer exists (the hwnd is invalid)
  * @par wParam:
  * Set to 0
  * @par lParam:
  * Set to 0
  * @ingroup bbapi_tray
  */
#define BB_CLEANTRAY            10508

#define BB_DRAGTODESKTOP        10510 /* wParam 0:execute drop  1:test if accepted */

/* ----------------------------------- */
/* ShellHook messages, obsolete - register for BB_TASKSUPDATE instead */
#define BB_ADDTASK              10601
#define BB_REMOVETASK           10602
#define BB_ACTIVATESHELLWINDOW  10603
#define BB_ACTIVETASK           10604
#define BB_MINMAXTASK           10605
#define BB_REDRAWTASK           10610

/* ----------------------------------- */
/* Window commands */
#define BB_WINDOWSHADE          10606 /* lParam: hwnd or NULL for foregroundwindow */
#define BB_WINDOWGROWHEIGHT     10607 /* ... */
#define BB_WINDOWGROWWIDTH      10608 /* ... */
#define BB_WINDOWLOWER          10609 /* ... */
#define BB_WINDOWMINIMIZE       10611 /* ... */
#define BB_WINDOWRAISE          10612 /* ... */
#define BB_WINDOWMAXIMIZE       10613 /* ... */
#define BB_WINDOWRESTORE        10614 /* ... */
#define BB_WINDOWCLOSE          10615 /* ... */
#define BB_WINDOWSIZE           10616 /* ... */
#define BB_WINDOWMOVE           10617 /* ... */

/* ----------------------------------- */
/* Broadcast a string (Bro@m) to core and all plugins */
#define BB_BROADCAST            10901 /* lParam: command string */

/* ----------------------------------- */
/* BBSlit messages */
#define SLIT_ADD                11001
#define SLIT_REMOVE             11002
#define SLIT_UPDATE             11003

/* ----------------------------------- */
/* Indices for pluginInfo(int index); */


/*=========================================================================== */
/* BBLean extensions */

/* for bbStylemaker & bbLeanSkin */
#define BB_REDRAWGUI            10881
/* wParam bitflags: */
#define BBRG_TOOLBAR (1<<0)
#define BBRG_MENU    (1<<1)
#define BBRG_WINDOW  (1<<2)
#define BBRG_DESK    (1<<3)
#define BBRG_FOCUS   (1<<4)
#define BBRG_PRESSED (1<<5)
#define BBRG_STICKY  (1<<6)
#define BBRG_FOLDER  (1<<7)

#define BB_EXECUTEASYNC         10882   /* Post a command or broam for execution (in lParam), like BB_EXECUTE, but returns immediately */
#define BB_DESKCLICK            10884   /* desktop clicked (lParam: 0=leftdown 1=left, 2=right, 3=mid, 4=x1, 5=x2, 6=x3) */
#define BB_WINKEY               10886   /* win9x: for bbKeys, left/right winkey pressed */

#define BB_GETSTYLE_OLD         10887   /* previous implementation, still here for bbstylemaker 1.2 */
#define BB_GETSTYLESTRUCT_OLD   10888   /* also */
#define BB_SETSTYLESTRUCT_OLD   10889   /* also */

#define BB_SENDDATA             10890
#define BB_GETSTYLE             10891   /* SendMessage(GetBBWnd(), BB_GETSTYLE, (WPARAM)buffer, (LPARAM)my_hwnd); */
#define BB_GETSTYLESTRUCT       10892   /* SendMessage(GetBBWnd(), BB_GETSTYLESTRUCT, (WPARAM)buffer, (LPARAM)my_hwnd); */
#define BB_SETSTYLESTRUCT       10893   /* etc. */

/* ----------------------------------- */
/* internal usage */

#ifdef __BBCORE__
#define BB_FOLDERCHANGED        10897   /* folder changed */
#define BB_DRAGOVER             10898   /* dragging over menu */
#define BB_POSTSTRING           10899   /* async command, for BB_EXECUTEASYNC */
#endif

/* ----------------------------------- */
/* done with BB_ messages */

/** @brief lParam: A new icon has been added to the tray
  * @ingroup bbapi_tray
  */
#define TRAYICON_ADDED 0
/** @brief lParam: An existing icon in the tray has changed
  * @ingroup bbapi_tray
  */
#define TRAYICON_MODIFIED 1
/** @brief lParam: An icon has been removed from the tray
  * @ingroup bbapi_tray
  */
#define TRAYICON_REMOVED 2

///@brief The lowest value a Blackbox window message will have
#define BB_MSGFIRST             10000
///@brief The highest value a Blackbox window message will have
#define BB_MSGLAST              13000

/*=========================================================================== */
/* extended Style-info for convenience and other purposes (backwards compatible) */

/** @brief Contains data for a single style item
  */
struct StyleItem
{
	/** @brief Style of the border on item
	  */
	int bevelstyle;
	/** @brief Style of the border on item
	  */
	int bevelposition;
	/** @brief Gradient type
	  */
	int type;
	/** @brief Indicates that only the border should be drawn when true
	  */
	bool parentRelative;
	/** @brief Inidcates if this item should be drawn interlaced
	  * @todo Get better description
	  */
	bool interlaced;

	/* 0.0.90 */
	COLORREF Color;
	COLORREF ColorTo;
	COLORREF TextColor;
	int FontHeight;
	int FontWeight;
	int Justify;
	int validated;

	/** @brief Font for this style item
	  *
	  * This member was originally 256 characters in bb4win. It has since been reduced to 128 characters,
	  * and the rest of the space used for all members that follow. The StyleItem.reserved member is used to keep
	  * the total size of a StyleItem at the same saize as the original bb4win implementation.
	  */
	char Font[128];

	/* below: experimental */
	int nVersion;
	int marginWidth;
	int borderWidth;
	COLORREF borderColor;
	COLORREF foregroundColor;
	COLORREF disabledColor;
	bool bordered;
	bool FontShadow; /* xoblite */
	union
	{
		struct {
			char ShadowX;
			char ShadowY;
		};
		unsigned short ShadowXY;
	};
	COLORREF ShadowColor;

	/** @brief Padding for structure size
	  *
	  * The size of this member is designed to keep the StyleItem structure at
	  * the same size as the original bb4win structure.
	  */
	char reserved[128 - (17*4 + 6)]; /* keep structure size */

};

#define picColor TextColor

/* StyleItem.validated flags */
#define VALID_TEXTURE       (1<<0)  /* gradient & bevel definition */

#define VALID_COLORFROM     (1<<1)  /* Color */
#define VALID_COLORTO       (1<<2)  /* ColorTo */
#define VALID_TEXTCOLOR     (1<<3)  /* TextColor */
#define VALID_PICCOLOR      (1<<4)  /* picColor */

#define VALID_FONT          (1<<5)  /* Font */
#define VALID_FONTHEIGHT    (1<<6)  /* FontHeight */
#define VALID_FONTWEIGHT    (1<<7)  /* FontWeight */
#define VALID_JUSTIFY       (1<<8)  /* Justify */

#define VALID_MARGIN        (1<<9)
#define VALID_BORDER        (1<<10)
#define VALID_BORDERCOLOR   (1<<11)

#define VALID_DISABLEDCOLOR (1<<12)

#define VALID_SHADOWCOLOR   (1<<13)
#define VALID_SHADOWX       (1<<14)
#define VALID_SHADOWY       (1<<15)

class Menu;
class MenuItem;

/** @brief Constants for use with GetSettingPtr.
  */
enum eSettings
{
	/** @brief Fetch the internal style structure
	  * @par GetSettingPtr will return
	  * StyleStruct *
	  * @warning This is an internal structure and is not defined in the API
	  */
	SN_STYLESTRUCT,
	/** @brief Fetch the toolbar style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_TOOLBAR,
	/** @brief Fetch the unpressed toolbar button style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_TOOLBARBUTTON,
	/** @brief Fetch the pressed toolbar button style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_TOOLBARBUTTONP,
	/** @brief Fetch the unpressed toolbar label style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_TOOLBARLABEL,
	/** @brief Fetch the toolbar windowlabel style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_TOOLBARWINDOWLABEL,
	/** @brief Fetch the toolbar clock style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_TOOLBARCLOCK,
	/** @brief Fetch the menu title style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_MENUTITLE,
	/** @brief Fetch the menu frame style (This is most of the menu)
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_MENUFRAME,
	/** @brief Fetch the menu highlight style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_MENUHILITE,

	SN_MENUBULLET               , /* char* */
	SN_MENUBULLETPOS              /* char* */
	,
	SN_BORDERWIDTH              , /* int* */
	SN_BORDERCOLOR              , /* COLORREF* */
	SN_BEVELWIDTH               , /* int* */
	SN_FRAMEWIDTH               , /* int* */
	SN_HANDLEHEIGHT             , /* int* */
	SN_ROOTCOMMAND              , /* char* */

	SN_MENUALPHA                , /* int* */
	SN_TOOLBARALPHA             , /* int* */
	SN_METRICSUNIX              , /* bool* */
	SN_BULLETUNIX               , /* bool* */
	/** @brief Fetch the title style for focused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  */
	SN_WINFOCUS_TITLE,
	/** @brief Fetch the label style for focused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINFOCUS_LABEL,
	/** @brief Fetch the handle style for focused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINFOCUS_HANDLE,
	/** @brief Fetch the grip style for focused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINFOCUS_GRIP,
	/** @brief Fetch the unpressed button style for focused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINFOCUS_BUTTON,
	/** @brief Fetch the pressed button style for focused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINFOCUS_BUTTONP,
	/** @brief Fetch the title style for unfocused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINUNFOCUS_TITLE,
	/** @brief Fetch the label style for unfocused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINUNFOCUS_LABEL,
	/** @brief Fetch the handle style for focused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINUNFOCUS_HANDLE,
	/** @brief Fetch the grip style for ufocused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINUNFOCUS_GRIP,
	/** @brief Fetch the unpressed button style for unfocused windows
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Defined under all releases except normal bb4win
	  * @note Unfocused windows only have an unpressed button style. Once you
	  * press the button the window will be focused.
	  */
	SN_WINUNFOCUS_BUTTON,
	/** @brief Fetch the frame color for focused windows
	  * @par GetSettingPtr will return
	  * COLORREF *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINFOCUS_FRAME_COLOR,
	/** @brief Fetch the frame color for unfocused windows
	  * @par GetSettingPtr will return
	  * COLORREF *
	  * @version Defined under all releases except normal bb4win
	  */
	SN_WINUNFOCUS_FRAME_COLOR,
	/** @brief Is the core/style using new metrics?
	  * @par GetSettingPtr will return
	  * bool
	  * @version Defined under all releases except normal bb4win
	  */
	SN_NEWMETRICS,
	/** @brief Fetch the menu seperator style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version Only present in recent bbleanmod, bb4win_mod.
	  * Defined by boxCore header but not implemented.
	  */
	SN_MENUSEPARATOR,
	/** @brief Fetch the menu volume slider style
	  * @par GetSettingPtr will return
	  * StyleItem *
	  * @version
	  * Present in recent bbleanmod, bb4win_mod and boxCore releases.
	  * Also present in bbClean releases but has a different value
	  */
	SN_MENUVOLUME,
	/** @brief One greater than the last valid constant
		  * @par GetSettingPtr will return
		  * Nothing
		  */
	SN_LAST
};

/*=========================================================================== */
/* Exported functions */

#ifdef __BBCORE__
#define API_EXPORT DLL_EXPORT
#else
#define API_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/* ------------------------------------ */
	/* Resource File API */

	/* Read Settings */
	API_EXPORT bool ReadBool(LPCSTR fileName, LPCSTR szKey, bool defaultBool);
	API_EXPORT int ReadInt(LPCSTR fileName, LPCSTR szKey, int defaultInt);
	API_EXPORT LPCSTR ReadString(LPCSTR fileName, LPCSTR szKey, LPCSTR defaultString);
	API_EXPORT COLORREF ReadColor(LPCSTR fileName, LPCSTR szKey, LPCSTR defaultString);

	/* Read a rc-value as string. If 'ptr' is specified, it can read a sequence of items with the same name. */
	API_EXPORT LPCSTR ReadValue(LPCSTR fileName, LPCSTR szKey, LPLONG ptr ISNULL);

	/* Was the last Value actually read from the rc-file ? */
	API_EXPORT int FoundLastValue(void);
	/* Returns: 0=not found, 1=found exact value, 2=found matching wildcard*/

	/* Write Settings */
	API_EXPORT void WriteBool(LPCSTR fileName, LPCSTR szKey, bool value);
	API_EXPORT void WriteInt(LPCSTR fileName, LPCSTR szKey, int value);
	API_EXPORT void WriteString(LPCSTR fileName, LPCSTR szKey, LPCSTR value);
	API_EXPORT void WriteColor(LPCSTR fileName, LPCSTR szKey, COLORREF value);

	/* Delete/Rename Setting */
	/// @ingroup bbapi_bblean
	API_EXPORT bool DeleteSetting(LPCSTR fileName, LPCSTR szKey); /* wildcards supported for keyword */
	/// @ingroup bbapi_bblean
	API_EXPORT bool RenameSetting(LPCSTR fileName, LPCSTR old_keyword, LPCSTR new_keyword);

	/* ------------------------------------ */
	/* Direct access to Settings variables / styleitems / colors */
	/* See the "SN_XXX" constants for possible values */
	API_EXPORT void* GetSettingPtr(int index);

	/* ------------------------------------ */
	/* File functions */
	API_EXPORT bool FileExists(LPCSTR fileName);
	API_EXPORT FILE *FileOpen(LPCSTR fileName);
	API_EXPORT bool FileClose(FILE *filePointer);
	API_EXPORT bool FileRead(FILE *filePointer, LPSTR buffer);
	API_EXPORT bool ReadNextCommand(FILE *filePointer, LPSTR buffer, DWORD maxLength);

	/// @ingroup bbapi_bblean
	API_EXPORT bool FindConfigFile(LPSTR pszOut, LPCSTR fileName, LPCSTR pluginDir);
	API_EXPORT LPCSTR ConfigFileExists(LPCSTR filename, LPCSTR pluginDir);

	/* ------------------------------------ */
	/* Make a window visible on all workspaces */
	API_EXPORT void MakeSticky(HWND window);
	API_EXPORT void RemoveSticky(HWND window);
	API_EXPORT bool CheckSticky(HWND window);

	/* ------------------------------------ */
	/* Window utilities */
	API_EXPORT int GetAppByWindow(HWND Window, char*);
	API_EXPORT bool IsAppWindow(HWND hwnd);

	/// @ingroup bbapi_bblean
	API_EXPORT HMONITOR GetMonitorRect(void *from, RECT *r, int Flags);
	/* Flags: */
#define GETMON_FROM_WINDOW 1    /* 'from' is HWND */
#define GETMON_FROM_POINT 2     /* 'from' is POINT* */
#define GETMON_FROM_MONITOR 4   /* 'from' is HMONITOR */
#define GETMON_WORKAREA 16      /* get working area rather than full screen */

	API_EXPORT void SnapWindowToEdge(WINDOWPOS* windowPosition, LPARAM nDist_or_pContent, UINT Flags);
	/* Flags: */
#define SNAP_FULLSCREEN 1  /* use full screen rather than workarea */
#define SNAP_NOPLUGINS  2  /* dont snap to other plugins */
#define SNAP_CONTENT    4  /* the "nDist_or_pContent" parameter points to a SIZE struct */
#define SNAP_NOPARENT   8  /* dont snap to parent window edges */
#define SNAP_SIZING    16  /* window is resized (bottom-right sizing only) */

	API_EXPORT bool SetTransparency(HWND hwnd, BYTE alpha);
	/* alpha: 0-255, 255=transparency off */

	/* ------------------------------------ */
	/* Desktop margins: */

	/* Add a screen margin at the indicated location */
	API_EXPORT void SetDesktopMargin(HWND hwnd, int location, int margin);

	/* with hwnd is the plugin's hwnd, location is one of the following: */
	enum { BB_DM_TOP, BB_DM_BOTTOM, BB_DM_LEFT, BB_DM_RIGHT };

	/* Dont forget to remove a margin before the plugin-window is destroyed. */
	/* Use SetDesktopMargin(hwnd, 0, 0) to remove it. */

	/* ------------------------------------ */
	/* Get paths */
	API_EXPORT void GetBlackboxEditor(LPSTR editor);
	API_EXPORT LPSTR WINAPI GetBlackboxPath(LPSTR path, int maxLength); /* with trailing backslash */

	API_EXPORT LPCSTR bbrcPath(LPCSTR bbrcFileName ISNULL);
	API_EXPORT LPCSTR extensionsrcPath(LPCSTR extensionsrcFileName ISNULL);
	API_EXPORT LPCSTR menuPath(LPCSTR menurcFileName ISNULL);
	API_EXPORT LPCSTR plugrcPath(LPCSTR pluginrcFileName ISNULL);
	API_EXPORT LPCSTR stylePath(LPCSTR styleFileName ISNULL);

	/* ------------------------------------ */
	/* Get the main window and other info */
	API_EXPORT HWND GetBBWnd();
	API_EXPORT LPCSTR GetBBVersion(); /* for instance: "bbLean 1.13" */
	API_EXPORT LPCSTR GetOSInfo();
	API_EXPORT bool GetUnderExplorer();

	/* ------------------------------------ */
	/* String tokenizing */
	API_EXPORT LPSTR Tokenize(LPCSTR sourceString, LPSTR targetString, LPCSTR delimiter);
	API_EXPORT int BBTokenize (LPCSTR sourceString, LPSTR* targetStrings, DWORD numTokensToParse, LPSTR remainingString);

	/* ------------------------------------ */
	/* Shell execute a command */
	API_EXPORT BOOL BBExecute(HWND Owner, LPCSTR szOperation, LPCSTR szCommand, LPCSTR szArgs, LPCSTR szDirectory, int nShowCmd, bool noErrorMsgs);

	/* ------------------------------------ */
	/* Logging and error messages */
	/* API_EXPORT void Log(LPCSTR format, ...); */
	API_EXPORT void Log(LPCSTR Title, LPCSTR Line);
	API_EXPORT int MBoxErrorFile(LPCSTR szFile);
	API_EXPORT int MBoxErrorValue(LPCSTR szValue);
	/// @ingroup bbapi_bblean
	API_EXPORT int BBMessageBox(int flg, const char *fmt, ...);

	/* debugging */
	API_EXPORT void dbg_printf(const char *fmt, ...);

	/* ------------------------------------ */
	/* Helpers */
	API_EXPORT void ParseItem(LPCSTR szItem, StyleItem *item);
	API_EXPORT bool IsInString(LPCSTR inputString, LPCSTR searchString);
	API_EXPORT LPSTR StrRemoveEncap(LPSTR string);
	API_EXPORT void ReplaceEnvVars(LPSTR string);
	API_EXPORT LPSTR ReplaceShellFolders(LPSTR path);

	/* ------------------------------------ */
	/* Painting */

	/* Generic Gradient Function */
	API_EXPORT void MakeGradient(HDC hdc, RECT rect, int gradientType, COLORREF colourFrom, COLORREF colourTo, bool interlaced, int bevelStyle, int bevelPosition, int bevelWidth, COLORREF borderColour, int borderWidth);
	/* Draw a Gradient Rectangle from StyleItem, optional using the style border. */
	API_EXPORT void MakeStyleGradient(HDC hDC, RECT* p_rect, StyleItem * m_si, bool withBorder);
	/* Draw a Border */
	API_EXPORT void CreateBorder(HDC hdc, RECT* p_rect, COLORREF borderColour, int borderWidth);
	/* Create a font handle from styleitem, with parsing and substitution. */
	API_EXPORT HFONT CreateStyleFont(StyleItem * si);
	/* Draw text with shadow, etc. */
	API_EXPORT int BBDrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat, StyleItem * si);

	/* ------------------------------------ */

	/* ------------------------------------ */
	/* Plugin Menu API - See the SDK for application examples */

	/* creates a Menu or Submenu, with an unique id, fshow indicates whether the menu is to be shown (true) or redrawn (false) */
	API_EXPORT Menu *MakeNamedMenu(LPCSTR HeaderText, LPCSTR Id, bool fshow);

	/* inserts an item to execute a command or to set a boolean value */
	API_EXPORT MenuItem *MakeMenuItem(Menu *PluginMenu, LPCSTR Title, LPCSTR Cmd, bool ShowIndicator);

	/* inserts an inactive item, optionally with text. 'Title' may be NULL. */
	API_EXPORT MenuItem *MakeMenuNOP(Menu *PluginMenu, LPCSTR Title ISNULL);

	/* inserts an item to adjust a numeric value */
	API_EXPORT MenuItem *MakeMenuItemInt(Menu *PluginMenu, LPCSTR Title, LPCSTR Cmd, int val, int minval, int maxval);

	/* inserts an item to edit a string value */
	API_EXPORT MenuItem *MakeMenuItemString(Menu *PluginMenu, LPCSTR Title, LPCSTR Cmd, LPCSTR init_string ISNULL);

	/* inserts an item, which opens a submenu */
	API_EXPORT MenuItem *MakeSubmenu(Menu *ParentMenu, Menu *ChildMenu, LPCSTR Title);

	/* inserts an item, which opens a submenu from a system folder.
	  'Cmd' optionally may be a Broam which is then sent on user click
	  with "%s" in the broam string replaced by the selected filename */
	/// @ingroup bbapi_bblean
	API_EXPORT MenuItem* MakePathMenu(Menu *ParentMenu, LPCSTR Title, LPCSTR path, LPCSTR Cmd ISNULL);

	/* set disabled state (menu.frame.disabledColor) for last added MenuItem */
	/// @ingroup bbapi_bblean
	API_EXPORT void DisableLastItem(Menu *PluginMenu);

	/* shows the menu */
	API_EXPORT void ShowMenu(Menu *PluginMenu);

	/* checks whether a menu is still on screen */
	/// @ingroup bbapi_bblean
	API_EXPORT bool MenuExists(LPCSTR IDString_start);

	/* ------------------------------------ */
	/* obsolete with MakeNamedMenu: */
	API_EXPORT Menu *MakeMenu(LPCSTR HeaderText);
	API_EXPORT void DelMenu(Menu *PluginMenu); /* does nothing */

	/* ------------------------------------ */

	/* ------------------------------------ */
	/* Tray icon access */
	/** @addtogroup bbapi_tray
	  * @{
	  */

	/** @brief Represents a single system tray icon
	  */
	struct systemTray
	{
		HWND    hWnd;
		UINT    uID;
		UINT    uCallbackMessage;
		HICON   hIcon;
		char    szTip[256 - 4];
		struct systemTrayBalloon *pBalloon; /* NULL when not present */
	};

	/** @brief Represents a bbLean balloon tooltip for a system tray icon
	  */
	struct systemTrayBalloon
	{
		UINT    uInfoTimeout;
		DWORD   dwInfoFlags;
		char    szInfoTitle[64];
		char    szInfo[256];
	};

	API_EXPORT int GetTraySize(void);
	API_EXPORT systemTray* GetTrayIcon(UINT pointer);

	/// @}

	/* experimental: */
//	typedef BOOL (*TRAYENUMPROC)(struct systemTray *, LPARAM);
//	API_EXPORT void EnumTray (TRAYENUMPROC lpEnumFunc, LPARAM lParam);

	/* ------------------------------------ */

	/* ------------------------------------ */
	/* Task items access */

	typedef struct tasklist /* current 0.90 compatibility outlay */
	{
		struct tasklist* next;
		HWND    hwnd;
		HICON   icon;
		int     wkspc;
		char    caption[244];
		HICON   orig_icon;
		bool    active;
		bool    flashing;
		bool    _former_hidden;
		bool    _former_reserved;
		HICON   _former_icon_big;
		HICON   icon_big;
		HICON   orig_icon_big;

	} tasklist;

	/* get the size */
	API_EXPORT int GetTaskListSize(void);
	/* get task's HWND by index */
	API_EXPORT HWND GetTask(int index);
	/* get the index of the currently active task */
	API_EXPORT int GetActiveTask(void);
	/* Retrieve a pointer to the internal TaskList. */
	API_EXPORT struct tasklist *GetTaskListPtr(void);

	typedef struct taskinfo
	{
		int xpos, ypos;     /* position */
		int width, height;  /* size, ignored with 'SetTaskLocation' */
		int desk;           /* workspace */
	} taskinfo;

	/* get workspace and original position/size for window */
	/// @ingroup bbapi_bblean
	API_EXPORT bool GetTaskLocation(HWND, struct taskinfo *pti);

	/* set workspace and/or position for window */
	/// @ingroup bbapi_bblean
	API_EXPORT bool SetTaskLocation(HWND, struct taskinfo *pti, UINT flags);
	/* where flags are: */
#define BBTI_SETDESK    1 /* move window to desk as specified */
#define BBTI_SETPOS     2 /* move window to x/ypos as spec'd */
#define BBTI_SWITCHTO   4 /* switch workspace after move */

	/* experimental: */
	typedef BOOL (*TASKENUMPROC)(struct tasklist *, LPARAM);
	API_EXPORT void EnumTasks (TASKENUMPROC lpEnumFunc, LPARAM lParam);

	/* ------------------------------------ */
	/* Get the workspace number for a task */
	API_EXPORT  int GetTaskWorkspace(HWND hwnd);

	/* For backwards compatibility only, depreciated! */
	API_EXPORT void SetTaskWorkspace(HWND hwnd, int workspace);

	/* ------------------------------------ */

	/* ------------------------------------ */
	/* Desktop Information: */
	typedef struct string_node {
		struct string_node *next;
		char str[1];
	} string_node;

	typedef struct DesktopInfo
	{
		char name[32];  /* name of the desktop */
		bool isCurrent; /* if it's the current desktop */
		int number;     /* desktop number */
		int ScreensX;   /* total number of screens */
		string_node *deskNames; /* list of all names */
	} DesktopInfo;

	/* Get the current Workspace number and name */
	API_EXPORT void GetDesktopInfo(DesktopInfo *deskInfo);
	/* Also, BB sends a BB_DESKTOPINFO message on workspace changes */
	/* with lParam pointing to a DesktopInfo structure */

	/* experimental: */
	typedef BOOL (*DESKENUMPROC)(struct DesktopInfo *, LPARAM);
	API_EXPORT void EnumDesks (DESKENUMPROC lpEnumFunc, LPARAM lParam);

	/* ------------------------------------ */

	/* ------------------------------------ */
	/* Get position and other info about the toolbar */

	typedef struct ToolbarInfo
	{
		/* rc - settings */
		int     placement;      /* 0 - 5 (top-left/center/right, bottom-left/center/right) */
		int     widthPercent;   /* 0 - 100 */
		bool    onTop;
		bool    autoHide;
		bool    pluginToggle;
		bool    disabled;       /* completely */

		bool    transparency;
		BYTE    alphaValue;     /* 0 - 255 (255 = transparency off) */

		/* current status */
		bool    autohidden;     /* currently by autoHide */
		bool    hidden;         /* currently by pluginToggle */
		int     xpos;
		int     ypos;
		int     width;
		int     height;
		HWND    hwnd;

		/* bbsb info, these are set by a systembar if present */
		HWND    bbsb_hwnd;
		bool    bbsb_linkedToToolbar;
		bool    bbsb_reverseTasks;
		bool    bbsb_currentOnly;
	} ToolbarInfo;

	/* retrieve a pointer to core's static ToolbarInfo */
	API_EXPORT ToolbarInfo *GetToolbarInfo(void);

	/* Toolbar placements */
#define PLACEMENT_TOP_LEFT 0
#define PLACEMENT_TOP_CENTER 1
#define PLACEMENT_TOP_RIGHT 2
#define PLACEMENT_BOTTOM_LEFT 3
#define PLACEMENT_BOTTOM_CENTER 4
#define PLACEMENT_BOTTOM_RIGHT 5

/// @}
	/* ------------------------------------ */
	/* plugin interface declarations */
/** @defgroup plugin Plugin Interface
  * @brief This sections lists the functions that a plugin should provide.
  *
  * The absolute minimum that a plugin can implement and still be loadable is beginPlugin(),
  * endPlugin() and pluginInfo(). Implementing beginPluginEx() and beginSlitPlugin() are
  * highly recommended even if the plugin does not use the slit, as some branches prefer these
  * functions when loading plugins.
  * @{
  */

/** @brief PluginInfo: Plugin should return its name
  */
#define PLUGIN_NAME         1
/** @brief PluginInfo: Plugin should return its version
  */
#define PLUGIN_VERSION      2
/** @brief PluginInfo: Plugin should return its author(s)
  */
#define PLUGIN_AUTHOR       3
/** @brief PluginInfo: Plugin should return its release date
  */
#define PLUGIN_RELEASE      4
/** @brief PluginInfo: Plugin should return its release date
  */
#define PLUGIN_RELEASEDATE  4   /* xoblite */
/** @brief PluginInfo: Plugin should return a link to its homepage
  */
#define PLUGIN_LINK         5
/** @brief PluginInfo: Plugin should return a contact email
  */
#define PLUGIN_EMAIL        6
/** @brief PluginInfo: Plugin should return a list of its bro@ms
  *
  * The should be one long string. Bro@@ms are split based on the @ so no spaces are needed between.
  * This is used by xoblite to generate a menu with the plugins bro@@ms in it.
  */
#define PLUGIN_BROAMS       7   /* xoblite */
/** @brief PluginInfo: Plugin should return a url where an update metafile can be found, if supported
  */
#define PLUGIN_UPDATE_URL   8   /* for Kaloth's BBPlugManager */

#ifndef __BBCORE__
	/** @brief Basic plugin startup function
	  * @param[in] hInstance The instance handle of the plugins DLL. Use when registering window classes etc.
	  *
	  * This is the original plugin entry function. When started through this function your plugin will not
	  * be able to use the slit or be loaded into bbInterface. It is a good idea to implement this for compatibility
	  * with all Blackbox versions. In many plugins this function just consists of
	  * @code
	  * beginPluginEx(hInstance, NULL);
	  * @endcode
	  */
	DLL_EXPORT int beginPlugin(HINSTANCE hInstance);

	/** @brief Slit plugin startup function
	  * @param[in] hInstance The instance handle of the plugins DLL. Use when registering window classes etc.
	  * @param[in] hSlit The window handle of the slit. May be NULL if the slit is not present.
	  *
	  * This is the original plugin entry function for plugins that supported the slit. The original intention
	  * was that plugins loaded via this function would always place themselves in the slit. Many
	  * plugins, however, only use the slit when it is enabled in their own config file.
	  * In many plugins this function just forwards to ::BeginPluginEx() and consists of
	  * @code
	  * beginPluginEx(hInstance, hSlit);
	  * @endcode
	  */
	DLL_EXPORT int beginSlitPlugin(HINSTANCE hInstance, HWND hSlit);
	/** @brief Flexible plugin startup function
	  * @param[in] hInstance The instance handle of the plugins DLL. Use when registering window classes etc.
	  * @param[in] hSlit The window handle of the slit. May be NULL if the slit is not present.
	  *
	  * This is the most recent addition to the plugin interface functions. When loaded with this function
	  * the slit information is available to a plugin. The plugin is expected to control whether it is located
	  * in the lsit or freestadning by checking its own config file.
	  */
	DLL_EXPORT int beginPluginEx(HINSTANCE hInstance, HWND hSlit);
	/** @brief Plugin shutdown function
	  * @param[in] hInstance The instance handle of the plugins DLL. Use to deregister window classes etc.
	  *
	  * This function is called just before the plugin will be unloaded. This function should make sure that
	  * all windows are destroyed, window classes unregistered and any other cleanup performed. Once
	  * you return from this function your plugin will not be able to execute anything else.
	  */
	DLL_EXPORT void endPlugin(HINSTANCE hInstance);
	/** @brief Pluginn information function
	  * @param[in] index The type of information the plugin should return. The value will be one of
	  * ::PLUGIN_NAME, ::PLUGIN_VERSION, ::PLUGIN_AUTHOR, ::PLUGIN_RELEASE, ::PLUGIN_LINK, ::PLUGIN_EMAIL,
	  * ::PLUGIN_BROAMS or ::PLUGIN_UPDATE_URL.
	  *
	  * This function should return the information requested by the index parameter. If the parameter
	  * is not one of the above values it is stadnard practice to return a string containing
	  * the plugin name and plugin version.
	  */
	DLL_EXPORT LPCSTR pluginInfo(int index);
/// @}
#endif

#ifdef __cplusplus
}
#endif

/*=========================================================================== */
#endif /* __BBAPI_H_ */
