#include "clsTrayItem.h"
#include <tchar.h>

#ifndef NIN_POPUPOPEN
#define NIN_POPUPOPEN 0x00000406
#endif
#ifndef NIN_POPUPCLOSE
#define NIN_POPUPCLOSE 0x00000407
#endif

clsTrayItem::clsTrayItem(systemTray *trayItem, UINT pIconSize, bool pVertical): clsIconItem(trayItem->hIcon, pIconSize, pVertical)
{
	if (TrayIconEvent == NULL)
	{
		if (bbApiLoader.requestApiPresence(TEXT("boxCore::hasTrayIconEvent")))
			TrayIconEvent = (fnTrayIconEvent)bbApiLoader.requestApiPointer("TrayIconEvent");
	}
	if (GetTrayInfo == NULL)
	{
		if (bbApiLoader.requestApiPresence(TEXT("boxCore::hasGetTrayInfo")))
			GetTrayInfo = (fnGetTrayInfo)bbApiLoader.requestApiPointer("GetTrayInfo");
	}
	iconWnd = trayItem->hWnd;
	iconID = trayItem->uID;
	m_version = 0;
	if (GetTrayInfo)
	{
		PRINT(TEXT("Got GetIconInfo"));
		PVOID info[2];
		ATOM infoTypes[2];
		infoTypes[0]=FindAtom(TEXT("TrayIcon::Version"));
#ifdef UNICODE
		infoTypes[1]=FindAtom(TEXT("TrayIcon::UnicodeTip"));
		PRINT(TEXT("We want Unicode tip"));
#else
		infoTypes[1]=FindAtom(TEXT("TrayIcon::AnsiTip"));
		PRINT(TEXT("We want ANSI tip"));
#endif
		if (GetTrayInfo(iconWnd,iconID,info,infoTypes,2))
		{
			m_version = reinterpret_cast<UINT_PTR>(info[0]);
			LPCTSTR szTip = reinterpret_cast<LPCTSTR>(info[1]);
			if (_tcslen(szTip))
			{
				tipText = new TCHAR[_tcslen(szTip)+1];
				_tcscpy(tipText,szTip);
			}
		}
	}
	else
	{
		if (strlen(trayItem->szTip))
		{
			tipText = new TCHAR[strlen(trayItem->szTip)+1];
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, trayItem->szTip, -1, tipText, strlen(trayItem->szTip) + 1);
#else
			strcpy(tipText, trayItem->szTip);
#endif
		}
	}
	iconCallback = trayItem->uCallbackMessage;
	popupVisible = false;
	itemAlpha = 150;
}

/** @brief wndProc
  *
  * @TODO: document this function
  */
LRESULT clsTrayItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
		if (popupVisible)
			break;
		popupVisible = true;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSELEAVE:
	{
		if (msg == WM_MOUSELEAVE)
			popupVisible = false;
		POINT mousePos;
		mousePos.x = LOWORD(lParam);
		mousePos.y = HIWORD(lParam);
		ClientToScreen(hWnd, &mousePos);
		LPARAM lParamNew = MAKELPARAM(mousePos.x, mousePos.y);
		if (TrayIconEvent)
			return TrayIconEvent(iconWnd, iconID, msg, wParam, lParamNew);
		else
		{
			if (IsWindow(iconWnd))
			{
				switch (m_version)
				{
				case 4:
					switch (msg)
					{
					case WM_RBUTTONUP:
						SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(WM_CONTEXTMENU, iconID));
						break;
					case WM_MOUSEMOVE:
						SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(NIN_POPUPOPEN, iconID));
						break;
					case WM_MOUSELEAVE:
						SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(NIN_POPUPCLOSE, iconID));
						break;
					default:
						SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(LOWORD(lParam), HIWORD(lParam)), MAKELPARAM(msg, iconID));
					}
					break;
				default:
					SendNotifyMessage(iconWnd, iconCallback, iconID, msg);
				}
				PRINT(TEXT("No TrayIconEvent"));
			}
			else
			{
				PostMessage(hBlackboxWnd, BB_CLEANTRAY, 0, 0);
			}
			return 0;
		}
	}
	case BB_TRAYUPDATE:
		switch (lParam)
		{
		case TRAYICON_MODIFIED:
			for (int i = 0; i < GetTraySize(); ++i)
			{
				systemTray *trayItem = GetTrayIcon(i);
				if ((trayItem->hWnd == iconWnd) && (trayItem->uID == iconID))
				{
					icon = trayItem->hIcon;
					delete tipText;
					tipText = NULL;
					if (GetTrayInfo)
					{
						PRINT(TEXT("Got GetIconInfo"));
						PVOID info[2];
						ATOM infoTypes[2];
						infoTypes[0]=FindAtom(TEXT("TrayIcon::Version"));
#ifdef UNICODE
						infoTypes[1]=FindAtom(TEXT("TrayIcon::UnicodeTip"));
						PRINT(TEXT("We want Unicode tip"));
#else
						infoTypes[1]=FindAtom(TEXT("TrayIcon::AnsiTip"));
						PRINT(TEXT("We want ANSI tip"));
#endif
						if (GetTrayInfo(iconWnd,iconID,info,infoTypes,2))
						{
							m_version = reinterpret_cast<UINT_PTR>(info[0]);
							LPCTSTR szTip = reinterpret_cast<LPCTSTR>(info[1]);
							if (_tcslen(szTip))
							{
								tipText = new TCHAR[_tcslen(szTip)+1];
								_tcscpy(tipText,szTip);
							}
						}
					}
					else
					{
						if (strlen(trayItem->szTip))
						{
							tipText = new TCHAR[strlen(trayItem->szTip)+1];
#ifdef UNICODE
							MultiByteToWideChar(CP_ACP, 0, trayItem->szTip, -1, tipText, strlen(trayItem->szTip) + 1);
#else
							strcpy(tipText, trayItem->szTip);
#endif

						}
					}

					setTooltip();
					InvalidateRect(barWnd, &itemArea, TRUE);
					PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
					return 0;
				}
			}
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

fnTrayIconEvent clsTrayItem::TrayIconEvent = NULL;
fnGetTrayInfo clsTrayItem::GetTrayInfo = NULL;
