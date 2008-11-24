#include "clsClockItem.h"
#include <ctime>
#include <tchar.h>

clsClockItem::clsClockItem(bool pVertical): clsLabelItem(pVertical)
{
	const CHAR *tempClockFormat;
	tempClockFormat = ReadString(configFile, "boxBar.clock.format:", "%#H:%M");
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, tempClockFormat, -1, clockFormat, 256);
#else
	strcpy(clockFormat, tempClockFormat);
#endif
	time_t systemTime;
	time(&systemTime);
	struct tm *ltp = localtime(&systemTime);
	_tcsftime(text, 256, clockFormat, ltp);
	ClockTimer = getTimerID();
	SetTimer(barWnd, ClockTimer, 1000, NULL);
}

clsClockItem::~clsClockItem()
{
	//dtor
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
			InvalidateRect(barWnd, &itemArea, TRUE);
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			bool seconds = _tcsstr(clockFormat, TEXT("%S")) || _tcsstr(clockFormat, TEXT("%#S"));
			SetTimer(barWnd, ClockTimer, seconds ? 1100 - lt.wMilliseconds : 61000 - lt.wSecond * 1000, NULL);
			calculateSizes();
			return 0;
		}
		break;
	case BB_RECONFIGURE:
		{
			const CHAR *tempClockFormat;
			tempClockFormat = ReadString(configFile, "boxBar.clock.format:", "%#H:%M");
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, tempClockFormat, -1, clockFormat, 256);
#else
			strcpy(clockFormat, tempClockFormat);
#endif
			SetTimer(barWnd, ClockTimer, 1000, NULL);
			calculateSizes();
			return 0;
		}
	default:
		return clsLabelItem::wndProc(hWnd, msg, wParam, lParam);
	}

	return clsLabelItem::wndProc(hWnd, msg, wParam, lParam);
}

