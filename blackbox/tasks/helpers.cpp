/*
 * helpers.cpp
 *
 *  Created on: 14 Feb 2009
 *      Author: shaun
 */

#include "helpers.h"

namespace TaskManagement
{

bool IsTask(HWND p_hWnd)
{
	if (!IsWindow(p_hWnd))
	{
		return false;
	}

	LONG_PTR style = GetWindowLongPtr(p_hWnd, GWL_STYLE);
	if ((style & WS_CHILD) || !IsWindowVisible(p_hWnd))
	{
		return false;
	}

	LONG_PTR exStyle = GetWindowLongPtr(p_hWnd, GWL_EXSTYLE);
	if (exStyle & WS_EX_APPWINDOW)
	{
		return true;
	}
	HWND owner = GetWindow(p_hWnd, GW_OWNER);
	if (owner)
	{
		return false;
	}
	if ((exStyle & WS_EX_TOOLWINDOW))
	{
		return false;
	}

	return true;
}

}
