/** @internal
  * @file
  * @brief This file contains the implementation the ShellServiceWindow class.
  *
  * ShellServiceWindow is part of the Shell Services implementation used by boxCore.
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

#include "clsShellServiceWindow.h"
#include "../../debug/debug.h"

namespace ShellServices
{

#ifndef MSGFLT_ADD
#define MSGFLT_ADD 1
#endif
#ifndef MSGFLT_REMOVE
#define MSGFLT_REMOVE 2
#endif

ShellServiceWindow::ShellServiceWindow(HINSTANCE pInstance, bool pTopmost)
{
	hInstance = pInstance;
	TaskbarCreated = RegisterWindowMessage(TEXT("TaskbarCreated"));
	hUser32 = LoadLibrary(TEXT("user32.dll"));
	if (hUser32)
	{
		ChangeWindowMessageFilter = (BOOL (*)(UINT, DWORD))GetProcAddress(hUser32, "ChangeWindowMessageFilter");
		if (ChangeWindowMessageFilter)
		{
			ChangeWindowMessageFilter(TaskbarCreated, MSGFLT_ADD);
		}
	}
	else
		ChangeWindowMessageFilter = NULL;

	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = sizeof(this);
	wndClass.lpszClassName = TEXT("Shell_TrayWnd");
	RegisterClassEx(&wndClass);

	hTrayWnd = CreateWindowEx( WS_EX_TOOLWINDOW | (pTopmost ? WS_EX_TOPMOST : 0), TEXT("Shell_TrayWnd"), NULL,
							   WS_POPUP | WS_DISABLED | WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							   0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	createChild(createChild(hTrayWnd, TEXT("TrayNotifyWnd")), TEXT("TrayClockWClass"));
	SetClassLongPtr(hTrayWnd, 0, (LONG_PTR)this);
	announceWindow();
}

ShellServiceWindow::~ShellServiceWindow()
{
	FreeLibrary(hUser32);
}

/** @brief Window procedure for children of Shell_TrayWnd
  *
  * Simply calls DefWindowProc, but can be expanded later if needed.
  */
LRESULT CALLBACK ShellServiceWindow::childWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/** @brief Window procedure for Shell_TrayWnd
  *
  * For WM_COPYDATA this executes handler if installed, otherwise returns FALSE.
  * For anything else we just call DefWindowProc.
  */
LRESULT CALLBACK ShellServiceWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ShellServiceWindow *owner = reinterpret_cast<ShellServiceWindow *>(GetClassLongPtr(hWnd, 0));

	switch (msg)
	{
	case WM_COPYDATA:
	{
		COPYDATASTRUCT *copyData = reinterpret_cast<COPYDATASTRUCT *>(lParam);
		if (owner->handlers[copyData->dwData])
			return owner->handlers[copyData->dwData]->ProcessMessage(copyData->cbData, copyData->lpData);
		else
		{
			TRACE("Unhandled COPYDATA msg %d", copyData->dwData);
			return FALSE;
		}
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

/** @brief Registers a handler for WM_COPYDATA messages
  *
  * @todo: document this function
  */
void ShellServiceWindow::RegisterHandler(eShellHandlers pType, ShellServiceHandler *pHandler)
{
	handlers[pType] = pHandler;
}

/** @brief Performs any notifications needed to make the presence of the new system tray known
  *
  * Broadcasts the message registered earlier for this purpose to all windows.
  */
void ShellServiceWindow::announceWindow()
{
	SendNotifyMessage(HWND_BROADCAST, TaskbarCreated, 0, 0);
}

/** @brief createChild
  *
  * @todo: document this function
  */
HWND ShellServiceWindow::createChild(HWND pParent, LPCTSTR pClsName)
{
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(wndClass);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = childWndProc;
	wndClass.lpszClassName = pClsName;
	RegisterClassEx(&wndClass);

	HWND childWindow = CreateWindowEx( 0, pClsName, NULL,
									   WS_CHILD | WS_DISABLED, 0, 0, 0, 0, pParent, NULL, hInstance, NULL);
	return childWindow;
}

}
