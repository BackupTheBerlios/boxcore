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

	itemAlpha = 255;

	itemBlend.BlendOp = AC_SRC_OVER;
	itemBlend.BlendFlags = 0;
	itemBlend.SourceConstantAlpha = itemAlpha;
	itemBlend.AlphaFormat = 0;

	ZeroMemory(&itemBitmapInfo, sizeof(itemBitmapInfo));
	itemBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	itemBitmapInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
	itemBitmapInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
	itemBitmapInfo.bmiHeader.biPlanes = 1;
	itemBitmapInfo.bmiHeader.biBitCount = 32;
}

/** @brief Base destructor
  *
  * Removes the tooltip, if it was set.
  */
clsItem::~clsItem()
{
	delete tipText;
	if (tipText)
	{
		tipText = NULL;
		setTooltip();
	}
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
		setTooltip();
}

/** @brief Base resizing funtion for items
  *
  * @param[in] pX The desired horizontal size
  * @param[in] pY The desired vertical size
  * @return Return the directions in which resizing actually occurred
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
	itemBitmapInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
	itemBitmapInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
	if (tipText)
		setTooltip();
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

/** @brief Base window procedure for items
  *
  * Handles mouse button messages and runs handlers if installed when
  * a click is detected. Passes all other message to DefWindowProc
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

/** @brief Sets a tooltip for this item
  *
  * Tooltip is set so it is triggered anywhere in the item area. We use the @b this pointer
  * as id, so we don't need extra storage to know which tooltip is ours. When called for the first time
  * in a plugin we call initTooltips to create a tooltip window to handle requests.
  */
void clsItem::setTooltip()
{
	if (!tooltipWnd)
		initTooltips();
	TOOLINFO toolInfo;
	ZeroMemory(&toolInfo, sizeof(toolInfo));
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.uFlags = TTF_SUBCLASS | TTF_TRANSPARENT;
	toolInfo.hwnd = barWnd;
	toolInfo.uId = (UINT_PTR)this;
	toolInfo.rect = itemArea;
	toolInfo.hinst = hInstance;
	toolInfo.lpszText = tipText;
	if (tipText)
	{
		SendMessage(tooltipWnd, TTM_DELTOOL, 0, (LPARAM)&toolInfo);
		SendMessage(tooltipWnd, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	}
	else
		SendMessage(tooltipWnd, TTM_DELTOOL, 0, (LPARAM)&toolInfo);
}

/** @brief Create a tooltip control for use by items
  *
  * This function creates a tooltip control window and then sets the parameter for that
  * window to values that are suitable for our use.
  * @note This is copied verbatim from bbLeanBars tooltip initalisation
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
  * @param[in,out] pContext The drawing context to use, passed on from the WM_PAINT message
  *
  * If the item has a style set, draws the stylegradient in the item area.
  */
void clsItem::draw(HDC pContext)
{
	//bool alphaDraw = false;
	if (style)
	{
		HDC internalDC;
		HBITMAP internalBitmap, origBitmap;
		if (alphaDraw)
		{
			internalDC = CreateCompatibleDC(pContext);
			internalBitmap = CreateDIBSection(internalDC, &itemBitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0);
			origBitmap = (HBITMAP) SelectObject(internalDC, internalBitmap);
			RECT tempArea;
			tempArea.left = tempArea.top = 0;
			tempArea.right = itemArea.right - itemArea.left;
			tempArea.bottom = itemArea.bottom - itemArea.top;
			MakeStyleGradient(internalDC, &tempArea, bbStyle.getStyle(style), bbStyle.getStyleBorder(style));
			msimg32.AlphaBlend(pContext, itemArea.left, itemArea.top,
							   itemArea.right - itemArea.left, itemArea.bottom - itemArea.top,
							   internalDC, 0, 0, itemArea.right - itemArea.left, itemArea.bottom - itemArea.top, itemBlend);
			SelectObject(internalDC, origBitmap);
			DeleteObject(internalBitmap);
			DeleteDC(internalDC);
		}
		else
		{
			MakeStyleGradient(pContext, &itemArea, bbStyle.getStyle(style), bbStyle.getStyleBorder(style));
		}

	}
}


/** @brief Stub for reading settings from the RC file
  *
  * Items should implement this if they need settings from the RC file. This stub does nothing,
  * and is present so that items can ignore this function if they do not need it.
  */
void clsItem::readSettings()
{
}

/** @brief Stub for config menu addition
  *
  * @param[in,out] pMenu Menu pointer to which items can be added
  *
  * This function should be implemented when an item wants to add something to the config menu.
  * Each item should handle message which these items will generate itself. This stub does nothing,
  * and is present so that items can ignore this function if they do not need it.
  */
void clsItem::configMenu(Menu *pMenu)
{
}

/** @brief Draws item immediately
  *
  * Retrieves a DC for the bar window and then draws the item to it. Use this
  * when an item updates, but does not move or change size.
  */
void clsItem::drawNow()
{
	HDC barDC = GetDC(barWnd);
	draw(barDC);
	ReleaseDC(barWnd, barDC);
}



clsApiLoader clsItem::bbApiLoader;
HWND clsItem::barWnd = NULL;
HWND clsItem::tooltipWnd = NULL;
CHAR clsItem::configFile[MAX_PATH] = {'\0'};
clsStyle clsItem::bbStyle;
HWND clsItem::hBlackboxWnd = NULL;
HINSTANCE clsItem::hInstance = NULL;
bool clsItem::alphaDraw = true;
