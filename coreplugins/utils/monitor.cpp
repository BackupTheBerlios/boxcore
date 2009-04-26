/*
 * monitor.cpp
 *
 *  Created on: 26 Apr 2009
 *      Author: shaun
 */

#include "../../dynwinapi/clsUser32.h"

#define MONITOR_DEFAULTTONEAREST 0x00000002

HMONITOR GetMonitorRect(HWND p_hWnd, RECT *p_rect)
{
	if (p_hWnd && user32.MonitorFromWindow)
	{
		HMONITOR mon = user32.MonitorFromWindow(p_hWnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO monInfo;
		ZeroMemory(&monInfo, sizeof(monInfo));
		monInfo.cbSize = sizeof(monInfo);
		user32.GetMonitorInfoA(mon, &monInfo);
		*p_rect = monInfo.rcMonitor;
		return mon;
	}
	else
	{
		p_rect->left = 0;
		p_rect->top = 0;
		p_rect->right = GetSystemMetrics(SM_CXSCREEN);
		p_rect->bottom = GetSystemMetrics(SM_CYSCREEN);
		return NULL;
	}
}
