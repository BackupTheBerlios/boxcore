#include "BBApi.h"
#include "clsLabelItem.h"
#include <tchar.h>

clsLabelItem::clsLabelItem(bool pVertical): clsItem(pVertical)
{
	fixed = DIM_BOTH;
	_tcscpy(text,TEXT("Label"));
}

clsLabelItem::~clsLabelItem()
{
	//dtor
}

/** @brief draw
  *
  * @todo: document this function
  */
void clsLabelItem::draw(HDC pContext)
{
	if (RectVisible(pContext, &itemArea))
	{
		/// @todo fix to not fetch each time
		MakeStyleGradient(pContext, &itemArea, bbStyle.getStyle(SN_TOOLBARCLOCK), bbStyle.getStyleBorder(SN_TOOLBARCLOCK));
		SetBkMode(pContext, TRANSPARENT);
		HFONT oldFont = (HFONT) SelectObject(pContext, bbStyle.getStyleFont(SN_TOOLBARCLOCK));
		COLORREF oldColor = SetTextColor(pContext, bbStyle.getStyleTextColor(SN_TOOLBARCLOCK));
		DrawText(pContext, text, -1, &itemArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		SetTextColor(pContext, oldColor);
		SelectObject(pContext, oldFont);
	}
}
/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsLabelItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_LBUTTON;
		break;
	case WM_RBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_RBUTTON;
		break;
	case WM_MBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_MBUTTON;
		break;
	case WM_XBUTTONDOWN:
		mouseDown = true;
		switch (HIWORD(wParam))
		{
		case XBUTTON1:
			mouseButton |= MK_XBUTTON1;
			break;
		case XBUTTON2:
			mouseButton |= MK_XBUTTON2;
			break;
		}
		break;
	case WM_LBUTTONUP:
		if (mouseDown && (mouseButton & MK_LBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			OutputDebugString(TEXT("Left Click in label"));
		}
		break;
	case WM_RBUTTONUP:
		if (mouseDown && (mouseButton & MK_RBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			OutputDebugString(TEXT("Right Click in label"));
		}
		break;
	case WM_MBUTTONUP:
		if (mouseDown && (mouseButton & MK_MBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			OutputDebugString(TEXT("Mid Click in label"));
		}
		break;
	case WM_XBUTTONUP:
		switch (HIWORD(wParam))
		{
		case XBUTTON1:
			if (mouseDown && (mouseButton & MK_XBUTTON1))
			{
				mouseButton = 0;
				mouseDown = false;
				OutputDebugString(TEXT("X1 Click in label"));
			}
			break;
		case XBUTTON2:
			if (mouseDown && (mouseButton & MK_XBUTTON2))
			{
				mouseButton = 0;
				mouseDown = false;
				OutputDebugString(TEXT("X1 Click in label"));
				break;
			}
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsLabelItem::calculateSizes(bool pSizeGiven)
{
	SIZE textSize;
	HDC tempDC = CreateCompatibleDC(NULL);
	HFONT oldFont = (HFONT) SelectObject(tempDC, bbStyle.getStyleFont(SN_TOOLBARCLOCK));
	GetTextExtentPoint32(tempDC, text, _tcslen(text), &textSize);
	SelectObject(tempDC, oldFont);
	DeleteDC(tempDC);
	bool enlarged = false;
	if (textSize.cx + 10 > minSizeX)
	{
	minSizeX = textSize.cx + 10;
	enlarged = true;
	}
	if (textSize.cy + 2>minSizeY)
	{
	minSizeY = textSize.cy + 2;
	enlarged=true;
	}
	if (enlarged)
	{
	resize(minSizeX, minSizeY);
	PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
	}
}







