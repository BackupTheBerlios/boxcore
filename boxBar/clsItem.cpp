#include "clsItem.h"
#include "commctrl.h"

clsItem::clsItem(bool pVertical)
{
	vertical = pVertical;
	itemArea.left = itemArea.right = itemArea.top = itemArea.bottom = 0;
	fixed = DIM_NONE;
	mouseDown = false;
	mouseButton = 0;
	minSizeX = 0;
	minSizeY = 0;
	tipText = NULL;
}

clsItem::~clsItem()
{
	//dtor
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
	switch(pDim)
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
		resize(minSizeX,minSizeY);
	}
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
	dbg_printf("Area : l %d r %d t %d b %d",itemArea.left, itemArea.right, itemArea.top, itemArea.bottom);
	TOOLINFO toolInfo;
	ZeroMemory(&toolInfo, sizeof(toolInfo));
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.uFlags = TTF_SUBCLASS;
	toolInfo.hwnd = barWnd;
	toolInfo.uId = (UINT_PTR)this;
	toolInfo.rect = itemArea;
	toolInfo.hinst = hInstance;
	toolInfo.lpszText = pText;
	SendMessage(tooltipWnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&toolInfo);
	SendMessage(tooltipWnd, TTM_ACTIVATE, TRUE, 0);
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



clsApiLoader clsItem::bbApiLoader;
HWND clsItem::barWnd = NULL;
HWND clsItem::tooltipWnd = NULL;
CHAR clsItem::configFile[MAX_PATH] = {'\0'};
clsStyle clsItem::bbStyle;
HWND clsItem::hBlackboxWnd = NULL;
HINSTANCE clsItem::hInstance = NULL;
