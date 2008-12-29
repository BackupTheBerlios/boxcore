#include "clsClockItem.h"
#include <ctime>
#include <tchar.h>
#include <locale.h>

clsClockItem::clsClockItem(bool pVertical): clsLabelItem(pVertical)
{
	style = SN_TOOLBARCLOCK;

	ClockTimer = getTimerID();
	readSettings();

	rightClick = showMenu;
	_tsetlocale(LC_ALL,L".ACP");
}

clsClockItem::~clsClockItem()
{
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsClockItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_TIMER:
		if (wParam == ClockTimer)
		{
			time_t systemTime;
			time(&systemTime);
			struct tm *ltp = localtime(&systemTime);
			_tcsftime(text, 256, clockFormat, ltp);
			drawNow();
			if (tipText)
				delete[] tipText;
			tipText = new TCHAR[256];
			_tcsftime(tipText, 256, clockTipFormat, ltp);
			setTooltip();
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			bool seconds = _tcsstr(clockFormat, TEXT("%S")) || _tcsstr(clockFormat, TEXT("%#S"));
			SetTimer(barWnd, ClockTimer, seconds ? 1100 - lt.wMilliseconds : 61000 - lt.wSecond * 1000, NULL);
			calculateSizes();
			InvalidateRect(barWnd, &itemArea, TRUE);
			PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
			return 0;
		}
		break;
	case BB_RECONFIGURE:
	{
		readSettings();
		calculateSizes();
		return 0;
	}
	}

	return clsLabelItem::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief showMenu
  *
  * @todo: document this function
  */
void clsClockItem::showMenu(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hBlackboxWnd, BB_BROADCAST, 0, (LPARAM)"@bbCore.ShowMenu");
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void clsClockItem::readSettings()
{
	const CHAR *tempClockFormat;
	tempClockFormat = ReadString(configFile, "boxBar.clock.format:", "%#H:%M");
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, tempClockFormat, -1, clockFormat, 256);
#else
	strcpy(clockFormat, tempClockFormat);
#endif
	tempClockFormat = ReadString(configFile, "boxBar.clock.tipformat:", "%A %d %B %Y");
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, tempClockFormat, -1, clockTipFormat, 256);
#else
	strcpy(clockTipFormat, tempClockFormat);
#endif
	SetTimer(barWnd, ClockTimer, 1, NULL);
}


