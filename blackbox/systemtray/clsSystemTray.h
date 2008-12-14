/** @internal
  * @file clsSystemTray.h
  * @brief This file contains the definition of clsSystemTray
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2008 Carsomyr -->
  * Copyright &copy; 2008 Carsomyr
  * @par links
  * http://developer.berlios.de/projects/boxcore @n
  * @par License
  * boxCore is free software, released under the GNU General
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
  *
  * The basic operations of this class is based on the functions in Tray.cpp in
  * the bbLean source code.
  */

#ifndef CLSSYSTEMTRAY_H
#define CLSSYSTEMTRAY_H

#include <shlwapi.h>
#include <windows.h>
#include <string>
#include <vector>
#include <list>

#include "clsTrayItem.h"

using std::vector;
using std::list;

enum trayCallbackType {TCALLBACK_ADD, TCALLBACK_MOD, TCALLBACK_DEL};

typedef std::basic_string<TCHAR> tstring;

/** @internal
  * @class clsSystemTray
  *
  * @brief Implementes system tray functionality (icon tracking etc)
  */
class clsSystemTray
{
	public:
		clsSystemTray(HINSTANCE &phInstance);
		void initialize();
		void terminate();
		virtual ~clsSystemTray();
		void announceSystemTray();
		void CleanTray();

		int GetNumVisible();
		const clsTrayItem *GetTrayIcon(int num);

		BOOL TrayIconEvent(HWND ownerHwnd, UINT iconID, UINT msg, WPARAM wParam, LPARAM lParam);
		void SetTaskbarPos(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge);

		void setCallback(trayCallbackType, void (*)());
	protected:
	private:
		static LRESULT CALLBACK trayWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK trayChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		HINSTANCE &hInstance;
		const tstring trayWndName;
		HWND hTrayWnd;
		HWND createTrayChild(const tstring pParentClass, const tstring pChildClass, const tstring pChildName = TEXT(""));
		HWND createTrayChild(HWND pParent, const tstring pChildClass, const tstring pChildName = TEXT(""));

		UINT trayCreatedMessage;

		vector<tstring> childClasses;
		vector<HWND> childWindows;
		list<clsTrayItem *> trayItems;

		void clearIconData(clsTrayItem *);
		LRESULT AddIcon(NID_INTERNAL &);
		LRESULT ModifyIcon(NID_INTERNAL &pNID, bool triggerCallback = true);
		LRESULT DeleteIcon(NID_INTERNAL &pNID, bool triggerCallback = true);
		LRESULT DeleteIcon(HWND pHwnd, UINT pID, bool triggerCallback = true);
		LRESULT SetIconVersion(NID_INTERNAL &);

		void (*callbackAdded)();
		void (*callbackModified)();
		void (*callbackDeleted)();

		static int barLeft, barRight, barTop, barBottom;
		static UINT barEdge;

		clsTrayItem * lookupIcon(HWND, UINT);

		HMODULE hUser32;
		BOOL (*ChangeWindowMessageFilter)(UINT message, DWORD dwFlag);

	struct NID_PRE2KA
	{
		DWORD cbSize;
		UINT hWnd;
		UINT uID;
		UINT uFlags;
		UINT uCallbackMessage;
		UINT hIcon;
		CHAR szTip[64];
	};

	struct NID_PRE2KW
	{
		DWORD cbSize;
		UINT hWnd;
		UINT uID;
		UINT uFlags;
		UINT uCallbackMessage;
		UINT hIcon;
		WCHAR szTip[64];
	};

	struct NID_2KA
	{
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
	};

	struct NID_2KW
	{
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
	};

	struct NID_XPA
	{
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
	};

	struct NID_XPW
	{
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
	};

	struct NID_VISTAA
	{
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
		UINT hBalloonItem;
	};

	struct NID_VISTAW
	{
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
		UINT hBalloonItem;
	};

};

#endif // CLSSYSTEMTRAY_H
