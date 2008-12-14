/** @internal
  * @file clsSystemTray.cpp
  * @brief This file contains the implementation of clsSystemTray
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

#include "clsSystemTray.h"
#include "../../dynwinapi/clsShlwapi.h"
#include "../../dynwinapi/clsUser32.h"

#ifndef MSGFLT_ADD
#define MSGFLT_ADD 1
#endif
#ifndef MSGFLT_REMOVE
#define MSGFLT_REMOVE 2
#endif

//IsTextUnicode(...) for testing for unicode text if needed
#ifndef NIF_INFO
#define NIF_INFO 0x00000010
#endif
#ifndef NIF_SHOWTIP
#define NIF_SHOWTIP 0x00000080
#endif
#ifndef NIIF_LARGE_ICON
#define NIIF_LARGE_ICON 0x00000020
#endif
#ifndef NIM_SETFOCUS
#define NIM_SETFOCUS 0x00000003
#endif
#ifndef NIM_SETVERSION
#define NIM_SETVERSION 0x00000004
#endif
#ifndef NIS_HIDDEN
#define NIS_HIDDEN 0x00000001
#endif
#ifndef NIS_SHAREDICON
#define NIS_SHAREDICON 0x00000002
#endif
#ifndef NIN_POPUPOPEN
#define NIN_POPUPOPEN 0x00000406
#endif
#ifndef NIN_POPUPCLOSE
#define NIN_POPUPCLOSE 0x00000407
#endif

clsShlwapi shlwapi;
clsUser32 user32;

//Recieved from XP onwards (lParam member is 64 bits)
struct APPBARDATA_64
{
	DWORD cbSize;
	///Really an HWND, but needed for spacing on 64 bit systems
	DWORD hWnd;
	UINT uCallbackMessage;
	UINT uEdge;
	RECT rc;
	LONG64 lParam;
};

//Recieved form 2k back, lParam member is 32 bits
struct APPBARDATA_32
{
	DWORD cbSize;
	///Really an HWND, but needed for spacing on 64 bit systems
	DWORD hWnd;
	UINT uCallbackMessage;
	UINT uEdge;
	RECT rc;
	LONG32 lParam;
};

/** @internal
  * Used in XP, Vista as well
  */
struct APPBARMSG_64
{
	APPBARDATA_64 abd;

	DWORD dwMessage;
	///Really a handle, but needed for correct spacing on 64 bit hosts
	DWORD hSharedMemory;
	DWORD dwSourceProcessID;
};


/** @internal
  * Used in 2k, and 98se
  */
struct APPBARMSG_32
{
	APPBARDATA_32 abd;

	DWORD dwMessage;
	///Really a handle, but needed for correct spacing on 64 bit hosts
	DWORD hSharedMemory;
	DWORD dwSourceProcessID;
};

/** @internal
  * We use a pointer to the 32 bit version of APPBARDATA, because we only need  the low order bits, and it keeps us safe
  */
struct INTERNAL_APPBARMSG
{
	APPBARDATA_32 *abd;

	DWORD dwMessage;
	HANDLE hSharedMemory;
	DWORD dwSourceProcessID;
};


/** @internal
  * @brief Format used by the shell in WM_COPYDATA messages to pass infomration to the system tray
  */
struct SHELLTRAYDATA
{
	DWORD dwMagic; // e.g. 0x34753423;
	DWORD dwMessage;
	NOTIFYICONDATA iconData;
};

/**
 * @brief Performs preperations for system tray creation
 *
 * The constructor registers the TaskbarCreated window message. The window message is also
 * added to the User Interface Privilage Isolation (UIPI) filter if present. This ensures
 * that the message will be able to reach all processes.
 */
clsSystemTray::clsSystemTray(HINSTANCE &phInstance): hInstance(phInstance), trayWndName(TEXT("Shell_TrayWnd"))
{
	trayCreatedMessage = RegisterWindowMessage(TEXT("TaskbarCreated"));
	hUser32 = LoadLibrary(TEXT("user32.dll"));
	if (hUser32)
	{
		ChangeWindowMessageFilter = (BOOL (*)(UINT, DWORD))GetProcAddress(hUser32, "ChangeWindowMessageFilter");
		if (ChangeWindowMessageFilter)
		{
			ChangeWindowMessageFilter(trayCreatedMessage, MSGFLT_ADD);
		}
	}
	else
		ChangeWindowMessageFilter = NULL;
	hTrayWnd = NULL;
	callbackAdded = NULL;
	callbackModified = NULL;
	callbackDeleted = NULL;
}

/**
 * @brief Cleans up after the system tray
 *
 * The destructor frees the reference to user32.dll which was used (or attempted to be used)
 * to load the UIPI filter function. The destructor also calls the terminate function to
 * clean up the system tray itself, if this has not already been done.
 */
clsSystemTray::~clsSystemTray()
{
	terminate();
	if (hUser32)
		FreeLibrary(hUser32);
}

/** @brief Performs initalization of the system tray
  *
  * We register a class and create a window to represent the system tray. Some
  * child windows are also created, as these exist in the real taskbar.
  * Once all windows are created, we broadcast the message to announce that the system tray
  * has been created.
  */
void clsSystemTray::initialize()
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = trayWndProc;
	wndClass.cbClsExtra = sizeof(this);
	wndClass.lpszClassName = trayWndName.c_str();
	RegisterClassEx(&wndClass);

	hTrayWnd = CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST, trayWndName.c_str(), NULL,
							   WS_POPUP | WS_DISABLED | WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							   0, 875, 1440, 25, NULL, NULL, hInstance, NULL);
	SetClassLongPtr(hTrayWnd, 0, (LONG_PTR)this);
	createTrayChild(trayWndName, TEXT("TrayNotifyWnd"));
	createTrayChild(TEXT("TrayNotifyWnd"), TEXT("TrayClockWClass"));
	createTrayChild(createTrayChild(TEXT("TrayNotifyWnd"), TEXT("SysPager")), TEXT("ToolbarWindow32"), TEXT("Notification Area"));
	createTrayChild(createTrayChild(TEXT("TrayNotifyWnd"), TEXT("SysPager")), TEXT("ToolbarWindow32"), TEXT("System Control Area"));
	//ShowWindow(hTrayWnd,SW_SHOW);

	announceSystemTray();
}

/** @brief Destroys the system tray and then cleans up
  *
  * Destroys the traywindow and all its children. Unregisters all the window classes that were
  * registered during initialization. Deletes all trayItems currently in the list after freeing the created icons.
  *
  */
void clsSystemTray::terminate()
{
	if (hTrayWnd)
	{
		DestroyWindow(hTrayWnd);
		hTrayWnd = NULL;

		UnregisterClass(trayWndName.c_str(), hInstance);
		for (unsigned int i = 0;i < childClasses.size();i++)
			UnregisterClass(childClasses[i].c_str(), hInstance);
	}

	for (list<clsTrayItem *>::iterator i = trayItems.begin();i != trayItems.end();i++)
	{
		clearIconData(*i);
		delete *i;
	}
}

/** @brief Register a child window class and creates an instance attached to the specified parent
  *
  * @param[in] pParentClass The clss name of the window which should be used as the parent
  * @param[in] pChildClass The class name that should be used for the child window
  *
  * We try to locate a parent window of teh requested class by querying the system. If this fails we
  * also consult the list of child windows we have. A window class is then registered with the new name
  * and a window of that class is created.
  */
HWND clsSystemTray::createTrayChild(const tstring pParentClass, const tstring pChildClass, const tstring pChildName)
{
	HWND parent = FindWindow(pParentClass.c_str(), NULL);

	if (parent == NULL)
	{
		for (unsigned int i = 0;i < childClasses.size();++i)
		{
			if (childClasses[i] == pParentClass)
			{
				parent = childWindows[i];
				break;
			}
		}
	}

	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = trayChildWndProc;
	wndClass.lpszClassName = pChildClass.c_str();
	RegisterClassEx(&wndClass);

	HWND childWindow = CreateWindowEx( 0, pChildClass.c_str(), pChildName.c_str(),
									   WS_CHILD | WS_DISABLED, 0, 0, 200, 25, parent, NULL, hInstance, NULL);
	childClasses.push_back(pChildClass);
	childWindows.push_back(childWindow);
	return childWindow;
}

/** @brief Register a child window class and creates an instance attached to the specified parent
  *
  * @param[in] pParentClass The clss name of the window which should be used as the parent
  * @param[in] pChildClass The class name that should be used for the child window
  *
  * We try to locate a parent window of teh requested class by querying the system. If this fails we
  * also consult the list of child windows we have. A window class is then registered with the new name
  * and a window of that class is created.
  */
HWND clsSystemTray::createTrayChild(HWND pParent, const tstring pChildClass, const tstring pChildName)
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = trayChildWndProc;
	wndClass.lpszClassName = pChildClass.c_str();
	RegisterClassEx(&wndClass);

	HWND childWindow = CreateWindowEx( 0, pChildClass.c_str(), pChildName.c_str(),
									   WS_CHILD | WS_DISABLED, 200, 200, 10, 10, pParent, NULL, hInstance, NULL);
	childClasses.push_back(pChildClass);
	childWindows.push_back(childWindow);
	return childWindow;
}


/** @brief Performs any notifications needed to make the presence of the new system tray known
  *
  * Broadcasts the message registered earlier for this purpose to all windows.
  */
void clsSystemTray::announceSystemTray()
{
	SendNotifyMessage(HWND_BROADCAST, trayCreatedMessage, 0, 0);
}

/** @brief Window procedure for the children of the tray
  *
  * Currently just calls DefWindowProc, additional functionality will be implemented if neccesary.
  */
LRESULT CALLBACK clsSystemTray::trayChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/** @brief Window procedure for the system tray window
  *
  * Calls DefWindowProc to handle everything except WM_COPYDATA.
  * @todo Document this pproperly
  */
LRESULT CALLBACK clsSystemTray::trayWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COPYDATA:
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	clsSystemTray *creator = (clsSystemTray *) GetClassLongPtr(hwnd, 0);

	switch (((COPYDATASTRUCT *)lParam)->dwData)
	{
	case 0: //Appbar stuff
		{
			char msg[100];
			/*sprintf(msg,"Size of struct %d we got. Size of 32 struct %d, size of 64 struct %d",((COPYDATASTRUCT *)lParam)->cbData, sizeof(APPBARMSGDATA32) ,sizeof(APPBARMSGDATA64));
			MessageBox(NULL,msg,"Appbar debug",MB_OK);
			INTERNAL_APPBARDATA myAppbarData;
			sprintf(msg,"Size of APPBAR %d, 32 is  %d, 64 is %d",((APPBARMSGDATA32*)((COPYDATASTRUCT *)lParam)->lpData)->abd.cbSize,sizeof(SH_APPBARDATA_32),sizeof(SH_APPBARDATA_64));
			MessageBox(NULL,msg,"Appbar debug",MB_OK);*/
			INTERNAL_APPBARMSG appbarMsg;
			//Always cast to 32 bit version for this assignment
			appbarMsg.abd = &(((APPBARMSG_32 *)((COPYDATASTRUCT *)lParam)->lpData)->abd);
			switch (((COPYDATASTRUCT *)lParam)->cbData)
			{
			case sizeof(APPBARMSG_64):
							appbarMsg.dwMessage = ((APPBARMSG_64 *)((COPYDATASTRUCT *)lParam)->lpData)->dwMessage;
				appbarMsg.hSharedMemory = (HANDLE)((APPBARMSG_64 *)((COPYDATASTRUCT *)lParam)->lpData)->hSharedMemory;
				appbarMsg.dwSourceProcessID = ((APPBARMSG_64 *)((COPYDATASTRUCT *)lParam)->lpData)->dwSourceProcessID;
				break;
			case sizeof(APPBARMSG_32):
							appbarMsg.dwMessage = ((APPBARMSG_32 *)((COPYDATASTRUCT *)lParam)->lpData)->dwMessage;
				appbarMsg.hSharedMemory = (HANDLE)((APPBARMSG_32 *)((COPYDATASTRUCT *)lParam)->lpData)->hSharedMemory;
				appbarMsg.dwSourceProcessID = ((APPBARMSG_32 *)((COPYDATASTRUCT *)lParam)->lpData)->dwSourceProcessID;
				break;
			default:
				return FALSE;
			}
			switch (appbarMsg.dwMessage)
	{
			case 0:
				return 1;
			case 5:
				if (shlwapi.SHLockShared)
					appbarMsg.abd = (APPBARDATA_32 *)shlwapi.SHLockShared(appbarMsg.hSharedMemory, appbarMsg.dwSourceProcessID);
				SetRect(&appbarMsg.abd->rc, barLeft, barTop, barRight, barBottom);
				appbarMsg.abd->uEdge = barEdge;
				if (shlwapi.SHUnlockShared)
					shlwapi.SHUnlockShared(appbarMsg.abd);
				return 1;
			default:
				sprintf(msg, "Appbar debug :The message is %ld or %ld", ((APPBARMSG_32*)((COPYDATASTRUCT *)lParam)->lpData)->dwMessage, ((APPBARMSG_64*)((COPYDATASTRUCT *)lParam)->lpData)->dwMessage);
				OutputDebugString(msg);
				return FALSE;
			}
		}
		return FALSE;
	case 1:
		{
			NID_INTERNAL NotifyIconData;
			NID_VISTAA *ansiNID = NULL;
			NID_VISTAW *unicodeNID = NULL;
			DWORD trayCommand;

			trayCommand =  ((SHELLTRAYDATA*)((COPYDATASTRUCT*)lParam)->lpData)->dwMessage;
			ansiNID    = (NID_VISTAA *) & ((SHELLTRAYDATA*)((COPYDATASTRUCT*)lParam)->lpData)->iconData;
			if ((ansiNID->cbSize == sizeof(NID_PRE2KW)) || (ansiNID->cbSize >= sizeof(NID_2KW)))
				unicodeNID = (NID_VISTAW *)ansiNID;

			ZeroMemory(&NotifyIconData, sizeof(NotifyIconData));
			//Copy always present part, no danger here
			if (unicodeNID)
			{
				NotifyIconData.hWnd = (HWND) unicodeNID->hWnd;
				NotifyIconData.uID = unicodeNID->uID;
				NotifyIconData.uFlags = unicodeNID->uFlags;
				NotifyIconData.uCallbackMessage = unicodeNID->uCallbackMessage;
				NotifyIconData.hIcon = (HICON) unicodeNID->hIcon;
			}
			else
			{
				NotifyIconData.hWnd = (HWND) ansiNID->hWnd;
				NotifyIconData.uID = ansiNID->uID;
				NotifyIconData.uFlags = ansiNID->uFlags;
				NotifyIconData.uCallbackMessage = ansiNID->uCallbackMessage;
				NotifyIconData.hIcon = (HICON) ansiNID->hIcon;
			}

			if (NotifyIconData.uFlags&NIF_TIP)
			{
				if (unicodeNID)
					lstrcpyW(NotifyIconData.szTip, unicodeNID->szTip);
				else
					MultiByteToWideChar(CP_ACP, 0, ansiNID->szTip, -1, NotifyIconData.szTip, 128);
			}

			if (NotifyIconData.uFlags&NIF_STATE)
			{
				if (unicodeNID)
				{
					NotifyIconData.dwState = unicodeNID->dwState;
					NotifyIconData.dwStateMask = unicodeNID->dwStateMask;
				}
				else
				{
					NotifyIconData.dwState = ansiNID->dwState;
					NotifyIconData.dwStateMask = ansiNID->dwStateMask;
				}
			}

			if (NotifyIconData.uFlags&NIF_INFO)
			{
				if (unicodeNID)
				{
					NotifyIconData.uTimeout = unicodeNID->uTimeout;
					NotifyIconData.dwInfoFlags = unicodeNID->dwInfoFlags;
					lstrcpyW(NotifyIconData.szInfo, unicodeNID->szInfo);
					lstrcpyW(NotifyIconData.szInfoTitle, unicodeNID->szInfoTitle);
				}
				else
				{
					NotifyIconData.dwState = ansiNID->dwState;
					NotifyIconData.dwStateMask = ansiNID->dwStateMask;
					MultiByteToWideChar(CP_ACP, 0, ansiNID->szInfo, -1, NotifyIconData.szInfo, 256);
					MultiByteToWideChar(CP_ACP, 0, ansiNID->szInfoTitle, -1, NotifyIconData.szInfoTitle, 64);
				}
			}

			if (trayCommand == NIM_SETVERSION)
			{
				if (unicodeNID)
				{
					NotifyIconData.uVersion = unicodeNID->uVersion;
				}
				else
				{
					NotifyIconData.uVersion = ansiNID->uVersion;
				}
			}

			if (NotifyIconData.dwInfoFlags&NIIF_LARGE_ICON)
			{
				if (unicodeNID)
					NotifyIconData.hBalloonItem = (HICON) unicodeNID->hBalloonItem;
				else
					NotifyIconData.hBalloonItem = (HICON) ansiNID->hBalloonItem;
			}

			switch (trayCommand)
			{
			case NIM_ADD:
				return creator->AddIcon(NotifyIconData);
			case NIM_MODIFY:
				return creator->ModifyIcon(NotifyIconData);
			case NIM_DELETE:
				return creator->DeleteIcon(NotifyIconData);
			case NIM_SETVERSION:
				return creator->SetIconVersion(NotifyIconData);
			case NIM_SETFOCUS:
			default:
				return DefWindowProc(hwnd, message, wParam, lParam);
			}
		}
		break;
	default:
		return FALSE;

	}
}
/** @brief lookupIcon
  *
  * @todo: document this function
  */
clsTrayItem * clsSystemTray::lookupIcon(HWND phWnd, UINT puID)
{
	for (list<clsTrayItem *>::iterator i = trayItems.begin();i != trayItems.end();i++)
	{
		if (((*i)->hWnd == phWnd) && ((*i)->iconID == puID))
			return *i;
	}
	return NULL;
}

/** @brief SetIconVersion
  *
  * @todo: document this function
  */
LRESULT clsSystemTray::SetIconVersion(NID_INTERNAL &pNID)
{
	if (IsWindow(pNID.hWnd) == FALSE)
	{
		DeleteIcon(pNID);
		return FALSE;
	}
	clsTrayItem *trayItem = lookupIcon(pNID.hWnd, pNID.uID);
	if (trayItem)
	{
		trayItem->version = pNID.uVersion;
		if (trayItem->version == 4)
			trayItem->showTooltip = false;
		else
			trayItem->showTooltip = true;
		return TRUE;
	}
	else
		return FALSE;
}

/** @brief DeleteIcon
  *
  * @todo: document this function
  */
LRESULT clsSystemTray::DeleteIcon(NID_INTERNAL &pNID, bool triggerCallback)
{
	clsTrayItem *trayItem = lookupIcon(pNID.hWnd, pNID.uID);
	if (trayItem)
	{
		bool wasHidden = trayItem->stateHidden;
		clearIconData(trayItem);
		///@todo Maybe something needed here for balloon tooltips
		trayItems.remove(trayItem);
		delete trayItem;
		if (!wasHidden && callbackDeleted && triggerCallback)
			callbackDeleted();
		return TRUE;
	}
	else
		return FALSE;
}

/** @brief DeleteIcon
  *
  * @todo: document this function
  */
LRESULT clsSystemTray::DeleteIcon(HWND pHwnd, UINT pID, bool triggerCallback)
{
	clsTrayItem *trayItem = lookupIcon(pHwnd, pID);
	if (trayItem)
	{
		bool wasHidden = trayItem->stateHidden;
		clearIconData(trayItem);
		///@todo Maybe something needed here for balloon tooltips
		trayItems.remove(trayItem);
		delete trayItem;
		if (!wasHidden && callbackDeleted && triggerCallback)
			callbackDeleted();
		return TRUE;
	}
	else
		return FALSE;
}

/** @brief ModifyIcon
  *
  * @todo: document this function
  */
LRESULT clsSystemTray::ModifyIcon(NID_INTERNAL &pNID, bool triggerCallback)
{
	if (IsWindow(pNID.hWnd) == FALSE)
	{
		DeleteIcon(pNID);
		return FALSE;
	}
	clsTrayItem *trayItem = lookupIcon(pNID.hWnd, pNID.uID);
	if (trayItem)
	{
		bool wasHidden = trayItem->stateHidden;
		if (pNID.uFlags & NIF_ICON)
		{
			///If a shared icon was requested, then we check whether there is an icon to share.
			///If the requested shared icon is not available, we mark the update as failed.
			///If the shared icon is valid, or it uses its own icon, we set it up accordingly.
			if ((pNID.uFlags&NIF_STATE) && (pNID.dwStateMask&pNID.dwState&NIS_SHAREDICON))
			{
				bool sharedValid = false;
				for (list<clsTrayItem *>::iterator i = trayItems.begin(); i != trayItems.end(); i++)
					if (!(*i)->stateShared && (*i)->hIconOrig == pNID.hIcon)
					{
						sharedValid = true;
						trayItem->hIcon = (*i)->hIcon;
						trayItem->hIconOrig = (*i)->hIconOrig;
						(*i)->iconRefcount++;
					}
				if (!sharedValid)
					return FALSE;
			}
			else
			{
				clearIconData(trayItem);
				trayItem->hIconOrig = pNID.hIcon;
				trayItem->hIcon = CopyIcon(pNID.hIcon);
			}
		}

		if (pNID.uFlags&NIF_TIP)
			trayItem->tooltip = pNID.szTip;

		if (pNID.uFlags&NIF_SHOWTIP)
			trayItem->showTooltip = true;

		if ((pNID.uFlags&NIF_STATE) && (pNID.dwStateMask&NIS_HIDDEN))
			trayItem->stateHidden = pNID.dwState & NIS_HIDDEN;

		if (triggerCallback)
		{
			if (trayItem->stateHidden)
			{
				if (!wasHidden)
				{
					if (callbackDeleted)
						callbackDeleted();
				}
			}
			else
			{
				if (wasHidden)
				{
					if (callbackAdded)
						callbackAdded();
				}
				else
				{
					if (callbackModified)
						callbackModified();
				}
			}
		}

		return TRUE;
	}
	else
	{
		///The Modify request is converted to an add request if the tray icon does not exist.
		///The add action will fail if insufficient information is present to create a new tray icon
		return AddIcon(pNID);
	}
}

/** @brief Function to handle a request to add an icon to the system tray
  *
  * We return false immediately if the icon being requested already exists.
  * If the icon does not yet exist, a new instance of clsTrayItem is created form the request.
  * The new tray icon is added to the list. The ModifyIcon fucntion is then called with the
  * information provided for icon creation. This ensures that we capture all the information from
  * the request, as the clsTrayItem constructor only copies the minimum information needed for an icon.
  *
  * @return FALSE is returned if an icon already exists, or if creation fails during any step
  * TRUE is returned is creation is succesfull in both the clsTrayItem constructor and ModifyIcon.
  */
LRESULT clsSystemTray::AddIcon(NID_INTERNAL &pNID)
{
	clsTrayItem *trayItem = lookupIcon(pNID.hWnd, pNID.uID);
	if (trayItem)
		return FALSE;
	else
	{
		trayItem = new clsTrayItem(pNID);
		if (trayItem->constructionValid())
		{
			trayItems.push_back(trayItem);
			if (TRUE == ModifyIcon(pNID, false))
			{
				if (!trayItem->stateHidden)
				{
					if (callbackAdded)
						callbackAdded();
				}
				return TRUE;
			}
			else
			{
				DeleteIcon(pNID, false);
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
}


/** @brief clearIconData
  *
  * @todo: document this function
  */
void clsSystemTray::clearIconData(clsTrayItem *pTrayItem)
{
	if (!pTrayItem->stateShared)
	{
		if (pTrayItem->iconRefcount != 0)
		{
			for (list<clsTrayItem *>::iterator i = trayItems.begin(); i != trayItems.end(); i++)
				if ((*i)->stateShared && (*i)->hIconOrig == pTrayItem->hIconOrig)
				{
					clearIconData(*i);
					pTrayItem->iconRefcount--;
				}
		}

		if (pTrayItem->hIcon) DestroyIcon(pTrayItem->hIcon);
	}
	pTrayItem->hIcon = NULL;
	pTrayItem->hIconOrig = NULL;
	///@todo Do we need to do something with a balloon icon?
}

/** @brief setCallback
  *
  * @todo: document this function
  */
void clsSystemTray::setCallback(trayCallbackType pType, void (*pCall)())
{
	switch (pType)
	{
	case TCALLBACK_ADD:
		callbackAdded = pCall;
		break;
	case TCALLBACK_MOD:
		callbackModified = pCall;
		break;
	case TCALLBACK_DEL:
		callbackDeleted = pCall;
		break;
	}
}

/** @brief CleanTray
  *
  * @todo: document this function
  */
void clsSystemTray::CleanTray()
{
	for (list<clsTrayItem *>::iterator i = trayItems.begin(); i != trayItems.end(); )
		if (IsWindow((*i)->hWnd) == FALSE)
		{
			delete (*i);
			i = trayItems.erase(i);
		}
		else
			i++;
}
/** @brief Fetches a tray item based on index
  * @param[in] num The index of the item to fetch
  * @return A read-only pointer to a tray item
  */
const clsTrayItem * clsSystemTray::GetTrayIcon(int num)
{
	int index = 0;
	for (list<clsTrayItem *>::iterator i = trayItems.begin(); i != trayItems.end(); i++)
	{
		if ((*i)->stateHidden == false)
		{
			if (index == num)
				return *i;
			else
				index++;
		}
	}
	return NULL;
}


/** @brief Get the number of visible icons in the tray
  * @return The number of visible icons in the system tray
  */
int clsSystemTray::GetNumVisible()
{
	int count = 0;
	for (list<clsTrayItem *>::iterator i = trayItems.begin(); i != trayItems.end(); i++)
	{
		if ((*i)->stateHidden == false)
		{
			count++;
		}
	}
	return count;
}

/** @brief Handles mouse events for system tray icons
  * @param[in] ownerHwnd The window handle of the icons owner
  * @param[in] iconID The icon id for the specific icon
  * @param[in] msg The message to be processed
  * @param[in] wParam The WPARAM value for the message
  * @param[in] lParam The LPARAM value for the message
  * @warning This function will be removed once version information is avialable via the api
  */
BOOL clsSystemTray::TrayIconEvent(HWND ownerHwnd, UINT iconID, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (IsWindow(ownerHwnd))
	{
		clsTrayItem *trayItem = lookupIcon(ownerHwnd, iconID);
		DWORD pid;
		if (WM_MOUSEMOVE != msg
				&& user32.AllowSetForegroundWindow
				&& GetWindowThreadProcessId(ownerHwnd, &pid))
		{
			user32.AllowSetForegroundWindow(pid);
		}
		if (trayItem)
		{
			switch (trayItem->version)
			{
			case 4:
				switch (msg)
				{
				case WM_RBUTTONUP:
					SendNotifyMessage(trayItem->hWnd, trayItem->callbackMessage, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(WM_CONTEXTMENU, trayItem->iconID));
					break;
				case WM_MOUSEMOVE:
					SendNotifyMessage(trayItem->hWnd, trayItem->callbackMessage, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(NIN_POPUPOPEN, trayItem->iconID));
					break;
				case WM_MOUSELEAVE:
					SendNotifyMessage(trayItem->hWnd, trayItem->callbackMessage, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(NIN_POPUPCLOSE, trayItem->iconID));
					break;
				default:
					SendNotifyMessage(trayItem->hWnd, trayItem->callbackMessage, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(msg, trayItem->iconID));
				}
				break;
			default:
				SendNotifyMessage(trayItem->hWnd, trayItem->callbackMessage, trayItem->iconID, msg);
			}
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		DeleteIcon(ownerHwnd, iconID);
		return FALSE;
	}
}

/** @brief SetTaskbarPos
  *
  * @todo: document this function
  */
void clsSystemTray::SetTaskbarPos(int pLeft, int pTop, int pRight, int pBottom, UINT pEdge)
{
	barLeft = pLeft;
	barRight = pRight;
	barTop = pTop;
	barBottom = pBottom;
	barEdge = pEdge;
	SetWindowPos(hTrayWnd, NULL, barLeft, barTop, barRight - barLeft, barBottom - barTop, SWP_NOZORDER | SWP_NOACTIVATE);
}


int clsSystemTray::barLeft = 0;
int clsSystemTray::barRight = 1440;
int clsSystemTray::barTop = 875;
int clsSystemTray::barBottom = 900;
UINT clsSystemTray::barEdge = ABE_BOTTOM;
