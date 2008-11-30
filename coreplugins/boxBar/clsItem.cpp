#include "clsItem.h"
#include "commctrl.h"

/** @brief Base constructor
  *
  * @param[in] pVertical Sets if this item is vertical or not
  *
  * Initializes all members to safe values, and sets an items vertical state as requested
  */
clsItem::clsItem(bool pVertical)
{
	vertical = pVertical;
	style = 0;
	itemArea.left = itemArea.right = itemArea.top = itemArea.bottom = 0;
	fixed = DIM_NONE;
	mouseDown = false;
	mouseButton = 0;
	minSizeX = 0;
	minSizeY = 0;
	tipText = NULL;

	leftClick = NULL;
	rightClick = NULL;
	midClick = NULL;
	X1Click = NULL;
	X2Click = NULL;
}

/** @brief Base destructor
  *
  * Removes the tooltip, if it was set.
  */
clsItem::~clsItem()
{
	if (tipText)
		setTooltip(NULL);
}

/** @brief Test if point is within the item
  *
  * For use on mouse events, when it is neccesary to determine if an item was clicked
  */
bool clsItem::hitTest(int pX, int pY)
{
	if ((pX >= itemArea.left) && (pX <= itemArea.right) && (pY >= itemArea.top) && (pY <= itemArea.bottom))
		return true;
	else
		return false;
}

/** @brief Base moving function for items
  *
  * Moves the item without any checks. Also moves the tootip location if one is set.
  * @todo This currently triggers a redraw at the new item area. Is this really needed?
  */
void clsItem::move(int pX, int pY)
{
	int sizeX = itemArea.right - itemArea.left;
	int sizeY = itemArea.bottom - itemArea.top;
	itemArea.left = pX;
	itemArea.right = pX + sizeX;
	itemArea.top = pY;
	itemArea.bottom = pY + sizeY;
	if (tipText)
		setTooltip(tipText);
	InvalidateRect(barWnd, &itemArea, TRUE);
}

/** @brief Base resizing funtion for items
  *
  * @param[in] pX The desired horizontal size
  * @param[in] pY The desired vertical size
  * @return Return the direcctions in which resizing actually occurred
  *
  * Resizes the item if the requested size is not negative.This allows a negative size
  * to be used to indicate that the old  size should be kept for that dimension.
  */
dimType clsItem::resize(int pX, int pY)
{
	dimType done = DIM_NONE;
	if (pX >= 0)
	{
		itemArea.right = itemArea.left + pX;
		done = DIM_HORIZONTAL;
	}
	if (pY >= 0)
	{
		itemArea.bottom = itemArea.top + pY;
		done = ((done == DIM_HORIZONTAL) ? DIM_BOTH : DIM_VERTICAL);
	}
	return done;
}

/** @brief Retrieve the size of an item
  *
  * @param[in] pDim The dimension of the size to return
  *
  * Returns the vertical or horizontal size of the item, as requested.
  */
int clsItem::getSize(dimType pDim)
{
	switch (pDim)
	{
	case DIM_HORIZONTAL:
		return itemArea.right - itemArea.left;
	case DIM_VERTICAL:
		return itemArea.bottom - itemArea.top;
	default:
		return -1;
	}
}

/** @brief Base sizing funtion for items
  *
  * @param[in] pSizegiven Indicates whether the size of the item has been set externally
  *
  * This base implementation sets the minimum size to 0 in both dimensions and then resizes
  * the item to that size. Calculations and sizing are only performed when the size
  * was not set externally.
  */
void clsItem::calculateSizes(bool pSizeGiven)
{
	if (!pSizeGiven)
	{
		minSizeX = 0;
		minSizeY = 0;
		resize(minSizeX, minSizeY);
	}
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			if (leftClick)
				leftClick(this, msg, wParam, lParam);
		}
		break;
	case WM_RBUTTONUP:
		if (mouseDown && (mouseButton & MK_RBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			if (rightClick)
				rightClick(this, msg, wParam, lParam);
		}
		break;
	case WM_MBUTTONUP:
		if (mouseDown && (mouseButton & MK_MBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			if (midClick)
				midClick(this, msg, wParam, lParam);
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
				if (X1Click)
					X1Click(this, msg, wParam, lParam);
			}
			break;
		case XBUTTON2:
			if (mouseDown && (mouseButton & MK_XBUTTON2))
			{
				mouseButton = 0;
				mouseDown = false;
				if (X2Click)
					X2Click(this, msg, wParam, lParam);
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/** @brief setTooltip
  *
  * @todo: document this function
  */
void clsItem::setTooltip(TCHAR *pText)
{
	if (!tooltipWnd)
		initTooltips();
	TOOLINFO toolInfo;
	ZeroMemory(&toolInfo, sizeof(toolInfo));
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.uFlags = TTF_SUBCLASS;
	toolInfo.hwnd = barWnd;
	toolInfo.uId = (UINT_PTR)this;
	toolInfo.rect = itemArea;
	toolInfo.hinst = hInstance;
	toolInfo.lpszText = pText;
	if (pText)
	{
		SendMessage(tooltipWnd, TTM_DELTOOL, 0, (LPARAM)&toolInfo);
		SendMessage(tooltipWnd, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	}
	else
		SendMessage(tooltipWnd, TTM_DELTOOL, 0, (LPARAM)&toolInfo);
}

/** @brief initTooltips
  * Copied from bbLeanbar
  * @todo: document this function
  */
void clsItem::initTooltips()
{
	INITCOMMONCONTROLSEX ic;
	ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ic.dwICC = ICC_BAR_CLASSES;

	InitCommonControlsEx(&ic);

	tooltipWnd = CreateWindowEx(
					 WS_EX_TOPMOST,
					 TOOLTIPS_CLASS,
					 NULL,
					 WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
					 CW_USEDEFAULT,
					 CW_USEDEFAULT,
					 CW_USEDEFAULT,
					 CW_USEDEFAULT,
					 barWnd,
					 NULL,
					 hInstance,
					 NULL);

	SetWindowPos(tooltipWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	SendMessage(tooltipWnd, TTM_SETMAXTIPWIDTH, 0, 300);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_AUTOMATIC, 300);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, 5000);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_INITIAL,  120);
	SendMessage(tooltipWnd, TTM_SETDELAYTIME, TTDT_RESHOW,    60);
}

/** @brief Base drawing function for items
  *
  * If the item has a style set, draws the stylegradient in the item area. Drawing is only performed
  * if the item area falls within the update area.
  */
void clsItem::draw(HDC pContext)
{
	if (style && RectVisible(pContext, &itemArea))
	{
		MakeStyleGradient(pContext, &itemArea, bbStyle.getStyle(style), bbStyle.getStyleBorder(style));
	}
}



clsApiLoader clsItem::bbApiLoader;
HWND clsItem::barWnd = NULL;
HWND clsItem::tooltipWnd = NULL;
CHAR clsItem::configFile[MAX_PATH] = {'\0'};
clsStyle clsItem::bbStyle;
HWND clsItem::hBlackboxWnd = NULL;
HINSTANCE clsItem::hInstance = NULL;
