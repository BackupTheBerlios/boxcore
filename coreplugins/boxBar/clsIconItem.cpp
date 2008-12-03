#include "clsIconItem.h"

clsIconItem::clsIconItem(HICON pIcon, UINT pSize, bool pVertical):clsItem(pVertical)
{
	icon = pIcon;
	iconSize = pSize;
	fixed = DIM_BOTH;
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsIconItem::calculateSizes(bool pSizeGiven)
{
	minSizeX = iconSize;
	minSizeY = iconSize;
	resize(minSizeX, minSizeY);
}

/** @brief draw
  *
  * @todo: document this function
  */
void clsIconItem::draw(HDC pContext)
{
	if (RectVisible(pContext, &itemArea))
	{
		DrawIconEx(pContext, itemArea.left, itemArea.top, icon, iconSize, iconSize, NULL, NULL, DI_NORMAL);
	}
}

/** @brief setIcon
  *
  * @todo: document this function
  */
void clsIconItem::setIcon(HICON pIcon)
{
	icon = pIcon;
	drawNow();
}


