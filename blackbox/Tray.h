/*
 ============================================================================

  This file is part of the bbLean source code
  Copyright � 2001-2003 The Blackbox for Windows Development Team
  Copyright � 2004 grischka

  http://bb4win.sourceforge.net/bblean
  http://sourceforge.net/projects/bb4win

 ============================================================================

  bbLean and bb4win are free software, released under the GNU General
  Public License (GPL version 2 or later), with an extension that allows
  linking of proprietary modules under a controlled interface. This means
  that plugins etc. are allowed to be released under any license the author
  wishes. For details see:

  http://www.fsf.org/licenses/gpl.html
  http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

 ============================================================================
*/

#ifndef __Tray_H
#define __Tray_H

#include <vector>
#include <shlobj.h>
#include <shellapi.h>
#ifdef __GNUC__
#include <shlwapi.h>
#endif
#include <docobj.h>

using std::vector;
typedef vector<IOleCommandTarget *> vecShellService;

//===========================================================================

void CleanTray(void);
void Tray_Init(void);
void Tray_Exit(void);

int GetTraySize();
systemTray* GetTrayIcon(int idx);

/* experimental: */
typedef BOOL (*TRAYENUMPROC)(struct systemTray *, LPARAM);
API_EXPORT void EnumTray (TRAYENUMPROC lpEnumFunc, LPARAM lParam);

/**
* @class coreTray
*
* @brief Implements the system tray for blackbox
*
* Loads shell service objects and handles the request for system tray icons from apps
*
* @author The bb4win devteam
*
*/
class coreTray
{
public:
	coreTray();
	
	~coreTray();
	void clean();
protected:
private:
	void loadShellServices();
	void loadShellServicesVista();
	
	vecShellService shellServiceList; ///Contains list of services which have been started
};

//===========================================================================
#ifdef INCLUDE_NIDS
#ifndef _WIN64
typedef struct _NID95 {
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	CHAR szTip[64];
} NID95;

typedef struct _NIDNT {
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	WCHAR szTip[64];
} NIDNT;

typedef struct _NID2K {
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	CHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	CHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	CHAR szInfoTitle[64];
	DWORD dwInfoFlags;
} NID2K;

typedef struct _NID2K6 {
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	CHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	CHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	CHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
} NID2K6;

typedef struct _NID2KW {
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
} NID2KW;

typedef struct _NID2KW6 {
	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
} NID2KW6;
#else
typedef struct _NID95 {
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	CHAR szTip[64];
} NID95;

typedef struct _NIDNT {
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	WCHAR szTip[64];
} NIDNT;

typedef struct _NID2K {
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	CHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	CHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	CHAR szInfoTitle[64];
	DWORD dwInfoFlags;
} NID2K;

typedef struct _NID2K6 {
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	CHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	CHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	CHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
} NID2K6;

typedef struct _NID2KW {
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
} NID2KW;

typedef struct _NID2KW6 {
	DWORD cbSize;
	UINT hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	UINT hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union {
		UINT uTimeout;
		UINT uVersion;
	};
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
} NID2KW6;
#endif

#endif

//===========================================================================
#endif
