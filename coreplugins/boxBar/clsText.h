#ifndef CLSTEXTITEM_H
#define CLSTEXTITEM_H

#include "clsItem.h"

namespace boxBar
{

class Text : public Item
{
public:
	Text(LPCSTR pText, UINT pStyle, dimType p_knowsSize = DIM_VERTICAL);
	Text(LPCWSTR pText, UINT pStyle, dimType p_knowsSize = DIM_VERTICAL);
	virtual ~Text();

	void SetText(LPCSTR pText);
	void SetText(LPCWSTR pText);
	void SetStyle(UINT pStyle);

	virtual void draw(HDC pContext);
	virtual void calculateSizes(bool pSizeGiven = false);
private:
	tstring m_text;

	StyleItem *m_styleItem;
	HFONT m_font;
};

}

#endif // CLSTEXTITEM_H
