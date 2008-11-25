#include "BBApi.h"
#include "clsItemCollection.h"

clsItemCollection::clsItemCollection(bool pVertical): clsItem(pVertical)
{
	if (vertical)
		fixed = DIM_HORIZONTAL;
	else
		fixed = DIM_VERTICAL;
	lastMouse = NULL;
}

clsItemCollection::~clsItemCollection()
{
	for (list<clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
		delete (*i);
	itemList.clear();
}

/** @brief draw
  *
  * @todo: document this function
  */
void clsItemCollection::draw(HDC pContext)
{
	if (RectVisible(pContext, &itemArea))
		for (list< clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
		{
			(*i)->draw(pContext);
		}
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsItemCollection::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
		for (list< clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
		{
			if ((*i)->hitTest(LOWORD(lParam), HIWORD(lParam)))
			{
				if (lastMouse && (lastMouse != (*i)))
					lastMouse->wndProc(hWnd, WM_MOUSELEAVE, wParam, lParam);
				lastMouse = (*i);
				return (*i)->wndProc(hWnd, msg, wParam, lParam);
			}

		}
		if (lastMouse)
		{
			lastMouse->wndProc(hWnd, WM_MOUSELEAVE, wParam, lParam);
			lastMouse = NULL;
		}
		break;
	case WM_TIMER:
		for (list< clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
		{
			(*i)->wndProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	if ((msg >= BB_MSGFIRST) && (msg <= BB_MSGLAST))
	{
		for (list< clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
		{
			(*i)->wndProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	return clsItem::wndProc(hWnd, msg, wParam, lParam);;
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsItemCollection::calculateSizes(bool pSizeGiven)
{
	dimType additiveDim, maxDim;
	fixedItemUsed = spacingBorder;
	int maxSize = 0;
	flexibleItemCount = 0;
	if (vertical)
	{
		additiveDim = DIM_VERTICAL;
		maxDim = DIM_HORIZONTAL;
	}
	else
	{
		additiveDim = DIM_HORIZONTAL;
		maxDim = DIM_VERTICAL;
	}

	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		(*i)->calculateSizes();
		dimType itemFixed = (*i)->getFixed();
		if (itemFixed & additiveDim)
		{
			int itemSize = (*i)->getSize(additiveDim);
			fixedItemUsed += itemSize + spacingItems;
		}
		else
		{
			fixedItemUsed += spacingItems;
			flexibleItemCount++;
		}
		if (itemFixed & maxDim)
		{
			int itemMax = (*i)->getSize(maxDim);
			if (itemMax > maxSize)
				maxSize = itemMax;
		}
	}
	fixedItemUsed += spacingBorder - spacingItems;

	if (pSizeGiven && (flexibleItemCount || (itemList.size()==0)))
	{
		if (itemList.size()==0)
			dbg_printf("We got nothin");
			minSizeY = getSize(DIM_VERTICAL);
			minSizeX = getSize(DIM_HORIZONTAL);
			if (vertical)
				maxSize = minSizeX - 2*spacingBorder;
			else
				maxSize = minSizeY - 2*spacingBorder;
	}
	else
	{
		if (vertical)
		{
			minSizeY = fixedItemUsed;
			minSizeX = maxSize + 2 * spacingBorder;
		}
		else
		{
			minSizeX = fixedItemUsed;
			minSizeY = maxSize + 2 * spacingBorder;
		}
	}
	if (flexibleItemCount)
	{

		if (vertical)
			flexibleItemSize = (minSizeY - fixedItemUsed) / flexibleItemCount;
		else
			flexibleItemSize = (minSizeX - fixedItemUsed) / flexibleItemCount;
		dbg_printf("Felxible items %d Flexible item size %d",flexibleItemCount, flexibleItemSize);
		for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
		{
			(*i)->calculateSizes();
			dimType itemFixed = (*i)->getFixed();
			if (!(itemFixed & additiveDim))
			{
				if (vertical)
					(*i)->resize(-1, flexibleItemSize);
				else
					(*i)->resize(flexibleItemSize, -1);
			}
		}
	}

	resize(minSizeX, minSizeY);
	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		if (vertical)
			(*i)->resize(maxSize, -1);
		else
			(*i)->resize(-1, maxSize);
		(*i)->calculateSizes(true);
	}
	sortItems();
}

/** @brief addItem
  *
  * @todo: document this function
  */
void clsItemCollection::addItem(clsItem *pItem)
{
	itemList.push_back(pItem);
}


/** @brief move
  *
  * @todo: document this function
  */
void clsItemCollection::move(int pX, int pY)
{
	clsItem::move(pX, pY);
	sortItems();
}


/** @brief sortItems
  *
  * @todo: document this function
  */
void clsItemCollection::sortItems()
{
	int pos;
	int available;
	if (vertical)
	{
		pos = itemArea.top + spacingBorder;
		available = itemArea.right - itemArea.left;
	}
	else
	{
		pos = itemArea.left + spacingBorder;
		available = itemArea.bottom - itemArea.top;
	}
	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		if (vertical)
		{

			(*i)->move(itemArea.left + (available - (*i)->getSize(DIM_HORIZONTAL)) / 2 , pos);
			pos += (*i)->getSize(DIM_VERTICAL) + spacingItems;
		}
		else
		{
			(*i)->move(pos, itemArea.top + (available - (*i)->getSize(DIM_VERTICAL)) / 2);
			pos += (*i)->getSize(DIM_HORIZONTAL) + spacingItems;
		}
	}
}



