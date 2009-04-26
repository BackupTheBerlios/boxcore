#include "BBApi.h"
#include "clsText.h"
#include <tchar.h>

namespace boxBar
{

Text::Text(LPCSTR pText, UINT pStyle, dimType p_knowsSize):
	Item("GenericText"),
	m_styleItem(NULL),
	m_font(NULL)
{
	m_knowsSize = p_knowsSize;
	m_wantsStretch = ((m_knowsSize & DIM_HORIZONTAL) ? DIM_NONE : DIM_HORIZONTAL);
	SetStyle(pStyle);
	if (pText)
	{
		CopyString(m_text, pText, 256);
	}
}

Text::Text(LPCWSTR pText, UINT pStyle, dimType p_knowsSize): Item()
{
	m_knowsSize = p_knowsSize;
	m_wantsStretch = ((m_knowsSize & DIM_HORIZONTAL) ? DIM_NONE : DIM_HORIZONTAL);
	SetStyle(pStyle);
	if (pText)
	{
		CopyString(m_text, pText, 256);
	}
}

Text::~Text()
{
	//dtor
}

/** @brief draw
  *
  * @todo: document this function
  */
void Text::draw(HDC pContext)
{
	HDC internalDC;
	HBITMAP alphaBitmap = NULL, oldBitmap = NULL;
	COLORREF oldColor;
	BYTE *ptPixels;
	//alphaDraw = false;
	if (alphaDraw)
	{
		internalDC = CreateCompatibleDC(pContext);
		BITMAPINFO bufferInfo;
		ZeroMemory(&bufferInfo, sizeof(bufferInfo));
		bufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bufferInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
		bufferInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
		bufferInfo.bmiHeader.biPlanes = 1;
		bufferInfo.bmiHeader.biBitCount = 32;
		alphaBitmap = CreateDIBSection(pContext, &bufferInfo, DIB_RGB_COLORS, (void **) & ptPixels, NULL, 0);
		oldBitmap = (HBITMAP) SelectObject(internalDC, alphaBitmap);
		oldColor = SetTextColor(internalDC, RGB(255,255,255));
	}
	else
	{
		internalDC = pContext;
		oldColor = SetTextColor(internalDC, m_styleItem->TextColor);
	}
	SetBkMode(internalDC, TRANSPARENT);
	HFONT oldFont = (HFONT) SelectObject(internalDC, m_font);
	RECT testRect = itemArea;
	DrawText(internalDC, m_text.c_str(), -1, &testRect, m_styleItem->Justify | DT_CALCRECT);
	if (testRect.right > itemArea.right)
		PostMessage(barWnd, BOXBAR_NEEDTIP, (WPARAM)m_text.c_str(), (LPARAM)this);
	else
		PostMessage(barWnd, BOXBAR_NEEDTIP, NULL, (LPARAM)this);
	testRect = itemArea;
	if (alphaDraw)
	{
		OffsetRect(&testRect, -itemArea.left, -itemArea.top);
	}
	DrawText(internalDC, m_text.c_str(), -1, &testRect, m_styleItem->Justify | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS | DT_NOPREFIX);
	SetTextColor(internalDC, oldColor);
	SelectObject(internalDC, oldFont);
	if (alphaDraw)
	{
		for (int i = 0; i < (testRect.bottom)*(testRect.right); ++i)
		{
			if (ptPixels[0] || ptPixels[1] || ptPixels[2])
			{
				ptPixels[3] = (ptPixels[0]*66 + ptPixels[1]*129 + ptPixels[2]*25)/256;
				ptPixels[0] = (GetBValue(m_styleItem->TextColor) * ptPixels[3]) / 256;
				ptPixels[1] = (GetGValue(m_styleItem->TextColor) * ptPixels[3]) / 256;
				ptPixels[2] = (GetRValue(m_styleItem->TextColor) * ptPixels[3]) / 256;
			}
			ptPixels+=4;
		}
		BLENDFUNCTION blendFunc;
		blendFunc.BlendOp = AC_SRC_OVER;
		blendFunc.BlendFlags = 0;
		blendFunc.SourceConstantAlpha = itemAlpha;
		blendFunc.AlphaFormat = AC_SRC_ALPHA;
		msimg32.AlphaBlend(pContext, itemArea.left, itemArea.top, itemArea.right - itemArea.left, itemArea.bottom - itemArea.top, internalDC,
						   0, 0, itemArea.right - itemArea.left, itemArea.bottom - itemArea.top, blendFunc);
		SelectObject(internalDC, oldBitmap);
		DeleteObject(alphaBitmap);
		DeleteDC(internalDC);
	}
}



/** @brief calculateSizes
  *
  * @todo: document this function
  */
void Text::calculateSizes(bool pSizeGiven)
{
	if (!pSizeGiven)
	{
		SIZE textSize;
		HDC tempDC = CreateCompatibleDC(NULL);
		HFONT oldFont = (HFONT) SelectObject(tempDC, m_font);
		GetTextExtentPoint32(tempDC, m_text.c_str(), m_text.size(), &textSize);
		SelectObject(tempDC, oldFont);
		DeleteDC(tempDC);
		resize(textSize.cx, textSize.cy);
	}
}

/** @brief setText
  *
  * @todo: document this function
  */
void Text::SetText(CONST CHAR *pText)
{
	CopyString(m_text, pText, 256);
	InvalidateRect(barWnd, &itemArea, TRUE);
	PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
}

/** @brief setText
  *
  * @todo: document this function
  */
void Text::SetText(CONST WCHAR *pText)
{
	CopyString(m_text, pText, 256);
	InvalidateRect(barWnd, &itemArea, TRUE);
	PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
}


/** @brief setStyle
  *
  * @todo: document this function
  */
void Text::SetStyle(UINT pStyle)
{
	if (m_font)
	{
		DeleteObject(m_font);
	}
	m_styleItem = reinterpret_cast<StyleItem *>(GetSettingPtr(pStyle));
	m_font = CreateStyleFont(reinterpret_cast<StyleItem *>(GetSettingPtr(SN_TOOLBAR)));
}

}
