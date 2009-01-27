/*
 * TrayForwarder.cpp
 *
 *  Created on: 24 Jan 2009
 *      Author: shaun
 */

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

HINSTANCE hInstance = NULL;
HWND hTrayWnd = NULL;
HWND hOurTrayWnd = NULL;
WNDPROC origWndProc = NULL;
UINT forwarderModuleMessage = 0;
bool moduleSent = false;

WINAPI DWORD DieProc(LPVOID p_param)
{
	OutputDebugString("Entering death throes");
	SetWindowLongPtr(hTrayWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(origWndProc));
	origWndProc = NULL;
	Sleep(1000);
	FreeLibraryAndExitThread(hInstance, 0);
}

LRESULT CALLBACK ForwarderProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == forwarderModuleMessage)
	{
		CreateThread(NULL, NULL, DieProc, NULL, NULL, NULL);
	}
	switch (uMsg)
	{
	case WM_NCDESTROY:
		SetWindowLongPtr(hTrayWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(origWndProc));
		break;
	case WM_COPYDATA:
	{
		COPYDATASTRUCT *copyData = reinterpret_cast<COPYDATASTRUCT *>(lParam);
		if (copyData->dwData == 1)
		{
			SendMessage(hOurTrayWnd, uMsg, wParam, lParam);
		}
		break;
	}
	}
	return CallWindowProc(origWndProc, hwnd, uMsg, wParam, lParam);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		moduleSent = false;
		hInstance = hinstDLL;
		hTrayWnd = FindWindow(TEXT("Shell_TrayWnd"), NULL);
		forwarderModuleMessage = RegisterWindowMessage(TEXT("TRAYFORWARDER_MODULE"));
		hOurTrayWnd = FindWindow(TEXT("HandlerShell_TrayWnd"), NULL);
		if (hOurTrayWnd)
		{
			origWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hTrayWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ForwarderProc)));
		}
		break;

	case DLL_PROCESS_DETACH:
		OutputDebugString("Detaching");
		//if (origWndProc)
		//{
		//SetWindowLongPtr(hTrayWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(origWndProc));
		//origWndProc = NULL;
		//}
		break;

	}
	return TRUE;
}
