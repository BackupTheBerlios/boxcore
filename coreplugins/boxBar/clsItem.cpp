#include "clsItem.h"
#include "commctrl.h"

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

clsItem::~clsItem()
{
	if (tipText)
		setTooltip(NULL);
}

/** @brief hitTest
  *
  * @todo: document this function
  */
bool clsItem::hitTest(int pX, int pY)
{
	if ((pX >= itemArea.left) && (pX <= itemArea.right) && (pY >= itemArea.top) && (pY <= itemArea.bottom))
		return true;
	else
		return false;
}

/** @brief move
  *
  * @todo: document this function
  */
void clsItem::move(int pX, int pY)
{
	//OutputDebugString(TEXT("Moved"));
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

/** @brief resize
  *
  * @todo: document this function
  */
dimType clsItem::resize(int pX, int pY)
{
	dimType done = DIM_NONE;
	if (minSizeX < 0)
		minSizeX = 0;
	if (minSizeY < 0)
		minSizeY = 0;
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

/** @brief getSize
  *
  * @todo: document this function
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

/** @brief calculateSizes
  *
  * @todo: document this function
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

/** draw
  *@todo document
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
