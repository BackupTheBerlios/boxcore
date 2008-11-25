#include "clsItem.h"

clsItem::clsItem(bool pVertical)
{
	vertical = pVertical;
	itemArea.left = itemArea.right = itemArea.top = itemArea.bottom = 0;
	fixed = DIM_NONE;
	mouseDown = false;
	mouseButton = 0;
	minSizeX = 0;
	minSizeY = 0;
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
	InvalidateRect(barWnd, &itemArea, TRUE);
}

/** @brief resize
  *
  * @todo: document this function
  */
dimType clsItem::resize(int pX, int pY)
{
	dimType done = DIM_NONE;
	if (pX >= minSizeX)
	{
		itemArea.right = itemArea.left + pX;
		done = DIM_HORIZONTAL;
	}
	if (pY >= minSizeY)
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

clsApiLoader clsItem::bbApiLoader;
HWND clsItem::barWnd = NULL;
CHAR clsItem::configFile[MAX_PATH] = {'\0'};
clsStyle clsItem::bbStyle;
HWND clsItem::hBlackboxWnd = NULL;
