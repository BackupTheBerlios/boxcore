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

	void setText(LPCSTR pText);
	void setText(LPCWSTR pText);
	void setStyle(UINT pStyle);

	virtual void draw(HDC pContext);
	virtual void calculateSizes(bool pSizeGiven = false);
protected:
	TCHAR text[256];
	UINT fontStyle;
};

}

#endif // CLSTEXTITEM_H
