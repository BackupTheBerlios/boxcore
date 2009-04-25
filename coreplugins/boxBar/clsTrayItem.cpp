#include "clsTrayItem.h"
#include <tchar.h>
#include "../../dynwinapi/clsUser32.h"
#include "clsTip.h"

#ifndef NIN_POPUPOPEN
#define NIN_POPUPOPEN 0x00000406
#endif
#ifndef NIN_POPUPCLOSE
#define NIN_POPUPCLOSE 0x00000407
#endif

namespace boxBar
{

TrayIcon::TrayIcon(systemTray *trayItem, UINT pIconSize): clsIconItem(trayItem->hIcon, pIconSize)
{
	if (GetTrayInfo == NULL)
	{
		if (bbApiLoader.requestApiPresence(TEXT("boxCore::hasGetTrayInfo")))
			GetTrayInfo = reinterpret_cast<fnGetTrayInfo>(bbApiLoader.requestApiPointer("GetTrayInfo"));
	}
	iconWnd = trayItem->hWnd;
	iconID = trayItem->uID;
	m_version = 0;
	if (GetTrayInfo)
	{
		PVOID info[2];
		ATOM infoTypes[2];
		infoTypes[0]=FindAtom(TEXT("TrayIcon::Version"));
#ifdef UNICODE
		infoTypes[1]=FindAtom(TEXT("TrayIcon::UnicodeTip"));
#else
		infoTypes[1]=FindAtom(TEXT("TrayIcon::AnsiTip"));
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
			CopyString(tipText, trayItem->szTip, strlen(trayItem->szTip)+1);
		}
	}
	iconCallback = trayItem->uCallbackMessage;
	popupVisible = false;
	itemAlpha = 255;
}

/** @brief wndProc
  *
  * @TODO: document this function
  */
LRESULT TrayIcon::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
		if ((msg == WM_LBUTTONDOWN) && GetAsyncKeyState(VK_MENU) & 0x8000)
		{
			CHAR className[256];
			CHAR windowName[256];
			GetClassNameA(iconWnd, className, 256);
			GetWindowTextA(iconWnd, windowName, 256);
			TRACE("You clicked icon belonging to %s of class %s", windowName, className);
			break;
		}
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
		POINT mousePos;
		mousePos.x = LOWORD(lParam);
		mousePos.y = HIWORD(lParam);
		ClientToScreen(hWnd, &mousePos);
		if (IsWindow(iconWnd))
		{
			DWORD pid;
			if (WM_MOUSEMOVE != msg
					&& user32.AllowSetForegroundWindow
					&& GetWindowThreadProcessId(iconWnd, &pid))
			{
				user32.AllowSetForegroundWindow(pid);
			}
			switch (m_version)
			{
			case 4:
				switch (msg)
				{
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPCLOSE, iconID));
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
					break;
				case WM_LBUTTONUP:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_USER, iconID));
					break;
				case WM_RBUTTONUP:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_CONTEXTMENU, iconID));
					break;
				case WM_MOUSEMOVE:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
					if (popupVisible)
						break;
					popupVisible = true;
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPOPEN, iconID));
					break;
				case WM_MOUSELEAVE:
					popupVisible = false;
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(NIN_POPUPCLOSE, iconID));
					break;
				default:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
				}
				break;
			case 3:
				switch (msg)
				{
				case WM_LBUTTONUP:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_USER, iconID));
					break;
				case WM_RBUTTONUP:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(WM_CONTEXTMENU, iconID));
					break;
				default:
					SendNotifyMessage(iconWnd, iconCallback, MAKEWPARAM(mousePos.x, mousePos.y), MAKELPARAM(msg, iconID));
				}
				break;
			default:
				SendNotifyMessage(iconWnd, iconCallback, iconID, msg);
			}
		}
		else
		{
			PostMessage(hBlackboxWnd, BB_CLEANTRAY, 0, 0);
		}
		return 0;
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
					setIcon(trayItem->hIcon);
					delete [] tipText;
					tipText = NULL;
					if (GetTrayInfo)
					{
						PVOID info[2];
						ATOM infoTypes[2];
						infoTypes[0]=FindAtom(TEXT("TrayIcon::Version"));
#ifdef UNICODE
						infoTypes[1]=FindAtom(TEXT("TrayIcon::UnicodeTip"));
#else
						infoTypes[1]=FindAtom(TEXT("TrayIcon::AnsiTip"));
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
							CopyString(tipText, trayItem->szTip, strlen(trayItem->szTip)+1);

						}
					}

					if (tipText)
					{
						if (!m_hasTooltip)
							setTooltip();
					}
					else
					{
						ClearTooltip();
					}

					if (trayItem->pBalloon && trayItem->pBalloon->uInfoTimeout && strlen(trayItem->pBalloon->szInfo))
					{
						POINT iconPos;
						iconPos.x = (itemArea.left + itemArea.right)/2;
						iconPos.y = (itemArea.top + itemArea.bottom)/2;
						ClientToScreen(hWnd, &iconPos);
						Tip *newTip = new Tip(hInstance, iconWnd,iconID,iconCallback, m_version,trayItem->pBalloon->szInfo,trayItem->pBalloon->szInfoTitle,trayItem->pBalloon->uInfoTimeout);
						newTip->Position(iconPos.x,iconPos.y);
						newTip->NotifyWindow(barWnd);
						trayItem->pBalloon->uInfoTimeout = 0;
						PostMessage(barWnd,BOXBAR_QUEUEBALLOON, 0, reinterpret_cast<LPARAM>(newTip));
					}
					InvalidateRect(barWnd, &itemArea, TRUE);
					PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
					return 0;
				}
			}
		}
	}
	return Item::wndProc(hWnd, msg, wParam, lParam);
}

fnGetTrayInfo TrayIcon::GetTrayInfo = NULL;

}
