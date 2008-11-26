#include "BBApi.h"
#include "clsTextItem.h"

clsTextItem::clsTextItem(TCHAR *pText, UINT pStyle, bool pVertical): clsItem(pVertical)
{
	fixed = DIM_VERTICAL;
	fontStyle = pStyle;
	_tcscpy(text,pText);
}

clsTextItem::~clsTextItem()
{
	//dtor
}

/** @brief draw
  *
  * @todo: document this function
  */
void clsTextItem::draw(HDC pContext)
{
	if (RectVisible(pContext, &itemArea))
	{
		SetBkMode(pContext, TRANSPARENT);
		HFONT oldFont = (HFONT) SelectObject(pContext, bbStyle.getStyleFont(fontStyle));
		COLORREF oldColor = SetTextColor(pContext, bbStyle.getStyleTextColor(fontStyle));
		DrawText(pContext, text, -1, &itemArea, bbStyle.getStyleTextJustify(fontStyle) | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
		SetTextColor(pContext, oldColor);
		SelectObject(pContext, oldFont);
	}
}



/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsTextItem::calculateSizes(bool pSizeGiven)
{
	if (!pSizeGiven)
	{
	SIZE textSize;
	HDC tempDC = CreateCompatibleDC(NULL);
	HFONT oldFont = (HFONT) SelectObject(tempDC, bbStyle.getStyleFont(fontStyle));
	GetTextExtentPoint32(tempDC, text, _tcslen(text), &textSize);
	SelectObject(tempDC, oldFont);
	DeleteDC(tempDC);
	minSizeX = textSize.cx;
	minSizeY = textSize.cy ;
	resize(minSizeX, minSizeY);
	}
}

/** @brief setText
  *
  * @todo: document this function
  */
void clsTextItem::setText(TCHAR *pText)
{
	_tcscpy(text,pText);
	InvalidateRect(barWnd, &itemArea, TRUE);
}


/** @brief setStyle
  *
  * @todo: document this function
  */
void clsTextItem::setStyle(UINT pStyle)
{
	fontStyle = pStyle;
	InvalidateRect(barWnd, &itemArea, TRUE);
}






