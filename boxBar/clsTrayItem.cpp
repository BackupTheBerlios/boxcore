#include "clsTrayItem.h"

clsTrayItem::clsTrayItem(systemTray *trayItem, bool pVertical): clsIconItem(trayItem->hIcon, 16, pVertical)
{
	if (TrayIconEvent == NULL)
	{
		if (bbApiLoader.requestApiPresence(L"boxCore::hasTrayIconEvent"))
			TrayIconEvent = (fnTrayIconEvent)bbApiLoader.requestApiPointer("TrayIconEvent");
	}
	iconWnd = trayItem->hWnd;
	iconID = trayItem->uID;
	iconCallback = trayItem->uCallbackMessage;
	if (strlen(trayItem->szTip))
	{
		dbg_printf("We have a tip");
	tipText = new TCHAR[strlen(trayItem->szTip)+1];
	MultiByteToWideChar(CP_ACP, 0, trayItem->szTip, -1, tipText, strlen(trayItem->szTip)+1);
	}
	popupVisible = false;
	fixed = DIM_BOTH;
	iconSize = ReadInt(configFile, "boxBar.tray.iconsize:", 16);
}

clsTrayItem::~clsTrayItem()
{
	//dtor
}

/** @brief wndProc
  *
  * @todo: document this function
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
		return TrayIconEvent(iconWnd, iconID, msg, wParam, lParamNew);
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
					//if (strlen(trayItem->szTip))
					//{
					//TCHAR mytip[256];
					//MultiByteToWideChar(CP_ACP, 0, trayItem->szTip, -1, mytip, 256);
					//setTooltip(mytip);
					//}
					InvalidateRect(hWnd, &itemArea, TRUE);
					return 0;
				}
			}
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

fnTrayIconEvent clsTrayItem::TrayIconEvent = NULL;
