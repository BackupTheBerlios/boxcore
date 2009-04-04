#ifndef CLSTEXTITEM_H
#define CLSTEXTITEM_H

#include "clsItem.h"

namespace Plugin_boxBar
{

class clsTextItem : public clsItem
{
public:
	clsTextItem(LPCSTR pText, UINT pStyle, bool pVertical, dimType p_knowsSize = DIM_VERTICAL);
	clsTextItem(LPCWSTR pText, UINT pStyle, bool pVertical, dimType p_knowsSize = DIM_VERTICAL);
	virtual ~clsTextItem();

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
