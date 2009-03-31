#include "BBApi.h"
#include "clsLabelItem.h"
#include <tchar.h>

clsLabelItem::clsLabelItem(bool pVertical) : clsTextItem(TEXT(""), SN_TOOLBARCLOCK, pVertical)
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
	style = SN_TOOLBARLABEL;
	_tcscpy(text,TEXT("Label"));
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







