#ifndef CLSTEXTITEM_H
#define CLSTEXTITEM_H

#include "clsItem.h"

namespace boxBar
{

class Text : public Item
{
public:
	template<typename T> Text(T pText, UINT pStyle, dimType p_knowsSize = DIM_VERTICAL);
	virtual ~Text();

	template<typename T> void SetText(T pText);
	void SetStyle(UINT pStyle);

	virtual void draw(HDC pContext);
	virtual void calculateSizes(bool pSizeGiven = false);
private:
	std::basic_string<WCHAR> m_text;

	StyleItem *m_styleItem;
	HFONT m_font;

	ULONG_PTR m_gdiplusToken;
};

}

#endif // CLSTEXTITEM_H
