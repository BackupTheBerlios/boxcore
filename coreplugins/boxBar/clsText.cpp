#include "BBApi.h"
#include "clsText.h"
#include <tchar.h>

#include <windows.h>
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#include <GdiPlus.h>
#undef min
#undef max

#include <cmath>

namespace boxBar
{

template<typename T>
Text::Text(T pText, UINT pStyle, dimType p_knowsSize):
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
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

}

template Text::Text(LPCSTR, UINT, dimType);
template Text::Text(LPSTR, UINT, dimType);
template Text::Text(LPCWSTR, UINT, dimType);
template Text::Text(LPWSTR, UINT, dimType);

Text::~Text()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

/** @brief draw
  *
  * @todo: document this function
  */
void Text::draw(HDC pContext)
{
	Gdiplus::Graphics canvas(pContext);
	Gdiplus::Font drawFont(pContext, m_font);
	Gdiplus::RectF drawRect(itemArea.left, itemArea.top, itemArea.right- itemArea.left, itemArea.bottom - itemArea.top);
	Gdiplus::Color brushColor(GetRValue(m_styleItem->TextColor),GetGValue(m_styleItem->TextColor),GetBValue(m_styleItem->TextColor));
	Gdiplus::SolidBrush brush(brushColor);
	Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap);
	format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	canvas.DrawString(m_text.c_str(), -1, &drawFont, drawRect, &format, &brush);
}



/** @brief calculateSizes
  *
  * @todo: document this function
  */
void Text::calculateSizes(bool pSizeGiven)
{
	if (!pSizeGiven)
	{
		HDC tempDC = CreateCompatibleDC(NULL);
		Gdiplus::Graphics canvas(tempDC);
		Gdiplus::Font drawFont(tempDC, m_font);
		Gdiplus::SizeF drawRect(99999999, 999999999);
		Gdiplus::SolidBrush brush(RGB(255,255,255));
		Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		Gdiplus::SizeF sizeNeeded;
		canvas.MeasureString(m_text.c_str(), -1, &drawFont, drawRect, &format, &sizeNeeded, NULL, NULL);
		resize(ceil(sizeNeeded.Width), ceil(sizeNeeded.Height));
		DeleteDC(tempDC);
	}
}

/** @brief setText
  *
  * @todo: document this function
  */
template<typename T>
void Text::SetText(T pText)
{
	CopyString(m_text, pText, 256);
	InvalidateRect(barWnd, &itemArea, TRUE);
	PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
}

template void Text::SetText(LPCSTR);
template void Text::SetText(LPSTR);
template void Text::SetText(LPCWSTR);
template void Text::SetText(LPWSTR);

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
