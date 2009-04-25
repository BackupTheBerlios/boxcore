#include "BBApi.h"
#include "clsCollection.h"
#include <algorithm>
#include <functional>

using std::for_each;
using std::mem_fun;
using std::bind2nd;

namespace Plugin_boxBar
{

/**
 * @page boxBarRCAdvanced
 * @section boxBarCollection Collection Item
 * These settings can be applied to several items.
 * These items include @ref boxBarTray "Tray", @ref boxBarTaskArea, @ref boxBarTask.
 * These settings can also be applied to the bar itself by removing "<item>." from the settings below. Otherwise replace "<item>"
 * with the item that you want to affect.
 * @code boxBar.<item>.Spacing.Border: <varies> @endcode
 * Space left unused around the border of this item, in pixels.
 * @code boxBar.<item>.Spacing.Items: <varies> @endcode
 * Space left between items, in pixels.
 */

Collection::Collection(bool pVertical, LPCSTR p_itemName, INT p_defaultBorder, INT p_defaultSpacing, minMaxStruct p_minMax) :
		Item(p_itemName, p_minMax),
		spacingBorder(s_settingsManager.AssociateInt(m_pluginPrefix, p_itemName, "Spacing.Border", p_defaultBorder)),
		spacingItems(s_settingsManager.AssociateInt(m_pluginPrefix, p_itemName, "Spacing.Items", p_defaultSpacing)),
		vertical(s_settingsManager.AssociateBool(m_pluginPrefix, p_itemName, "Vertical", pVertical))
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
	lastMouse = NULL;
	isBar = false;
}

Collection::~Collection()
{
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
		delete (*i);
	itemList.clear();
}

/** @brief Base draw function for collections fo items
 *
 * @param[in,out] pContext The drawing context to use, passed on from the WM_PAINT message
 *
 * Calls the clsItem::draw in case a style was set, then calls draw for each item in the list, passing on the DC.
 */
void Collection::draw(HDC pContext)
{
	if (style)
	{
		HDC internalDC;
		HBITMAP internalBitmap, origBitmap;
		if (alphaDraw)
		{
			internalDC = CreateCompatibleDC(pContext);
			internalBitmap = CreateDIBSection(internalDC, &itemBitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0);
			origBitmap = (HBITMAP) SelectObject(internalDC, internalBitmap);
			RECT tempArea = itemArea;
			OffsetRect(&tempArea, -itemArea.left, -itemArea.top);
			BitBlt(internalDC, tempArea.left, tempArea.top, tempArea.right, tempArea.bottom,
				   pContext, itemArea.left, itemArea.top, SRCCOPY);
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
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		(*i)->draw(pContext);
	}
}

/** @brief wndProc
 *
 * @todo: document this function
 */
LRESULT Collection::wndProc(HWND hWnd, UINT msg, WPARAM wParam,
							LPARAM lParam)
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
		for (itemList_t::iterator i = itemList.begin(); i
				!= itemList.end(); ++i)
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
	case BOXBAR_NEEDTIP:
	case WM_TIMER:
	case WM_NOTIFY:
		for (itemList_t::iterator i = itemList.begin(); i
				!= itemList.end(); ++i)
		{
			(*i)->wndProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}

	if ((msg >= BB_MSGFIRST) && (msg <= BB_MSGLAST))
	{
		for (itemList_t::iterator i = itemList.begin(); i
				!= itemList.end(); ++i)
		{
			(*i)->wndProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}
	return Item::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void Collection::calculateSizes(bool pSizeGiven)
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

	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		(*i)->calculateSizes();
		dimType itemFixed = (*i)->getKnowsSize();
		if (itemFixed & additiveDim)
		{
			int itemSize = (*i)->getSize(additiveDim);
			fixedItemUsed += itemSize + spacingItems;
		}
		else
		{
			fixedItemUsed += spacingItems;
		}

		if (additiveDim && (*i)->getWantsStretch())
		{
			flexibleItemCount++;
		}

		if (itemFixed & maxDim)
		{
			int itemMax = (*i)->getSize(maxDim);
			maxSize = std::max(maxSize, itemMax);
		}
	}
	fixedItemUsed += spacingBorder - spacingItems;

	if (pSizeGiven && ((flexibleItemCount || (itemList.size() == 0)) || !isBar))
	{
		minSizeY = std::max(getSize(DIM_VERTICAL),m_minSizeY);
		minSizeX = std::max(getSize(DIM_HORIZONTAL),m_minSizeX);
		if (vertical)
		{
			maxSize = std::max(maxSize, minSizeX - 2 * spacingBorder);
			minSizeX = maxSize + 2 * spacingBorder;
		}
		else
		{
			maxSize = std::max(maxSize, minSizeY - 2 * spacingBorder);
			minSizeY = maxSize + 2 * spacingBorder;
		}

	}
	else
	{
		if (vertical)
		{
			minSizeY = std::max(fixedItemUsed, m_minSizeY);
			minSizeX = std::max(maxSize + 2 * spacingBorder, m_minSizeX);
			maxSize = minSizeX - 2 * spacingBorder;
		}
		else
		{
			minSizeX = std::max(fixedItemUsed, m_minSizeX);
			minSizeY = std::max(maxSize + 2 * spacingBorder, m_minSizeY);
			maxSize = minSizeY - 2 * spacingBorder;
		}
	}
	if (flexibleItemCount)
	{

		if (vertical)
			flexibleItemSize = (minSizeY - fixedItemUsed) / flexibleItemCount;
		else
			flexibleItemSize = (minSizeX - fixedItemUsed) / flexibleItemCount;
		flexibleItemSize = std::max(flexibleItemSize, 0);
		for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
		{
			(*i)->calculateSizes();
			dimType itemStretch = (*i)->getWantsStretch();
			if (itemStretch & additiveDim)
			{
				if (vertical)
					(*i)->resize(-1, flexibleItemSize);
				else
					(*i)->resize(flexibleItemSize, -1);
			}
		}
	}

	resize(minSizeX, minSizeY);
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
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
void Collection::addItem(Item *p_item, bool p_front)
{
	if (p_front)
	{
		itemList.push_front(p_item);
	}
	else
	{
		itemList.push_back(p_item);
	}
}

/** @brief Changes the position of the collection and its contents
  *
  * This function calls clsItem::move() to move the collection itself and then
  * uses sortItems() to reposition the contents of the collection.
  */
void Collection::move(int pX, int pY)
{
	Item::move(pX, pY);
	sortItems();
}

/** @brief sortItems
 *
 * @todo: document this function
 */
void Collection::sortItems()
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
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		if (vertical)
		{
			(*i)->move(itemArea.left + (available
										- (*i)->getSize(DIM_HORIZONTAL)) / 2, pos);
			pos += (*i)->getSize(DIM_VERTICAL) + spacingItems;
		}
		else
		{
			(*i)->move(pos, itemArea.top + (available
											- (*i)->getSize(DIM_VERTICAL)) / 2);
			pos += (*i)->getSize(DIM_HORIZONTAL) + spacingItems;
		}
	}
}

/** @brief Check contained items for configuration menus
 *
 * @param[in,out] pMenu Menu pointer to which items can be added
 *
 * This calls configMenu on each contained item
 */
void Collection::configMenu(Menu *pMenu, bool p_update)
{
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		(*i)->configMenu(pMenu, p_update);
	}
	//for_each(itemList.begin(), itemList.end(),
	//		 bind2nd(mem_fun((void(clsItem::*)(Menu*)) &clsItem::configMenu),
	//				 pMenu));
}

}
