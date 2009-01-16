#ifndef CLSTEXTITEM_H
#define CLSTEXTITEM_H

#include "clsItem.h"


class clsTextItem : public clsItem
{
public:
	clsTextItem(CONST TCHAR *pText, UINT pStyle, bool pVertical, dimType p_knowsSize = DIM_VERTICAL);
	virtual ~clsTextItem();

	void setText(CONST CHAR *pText);
	void setText(CONST WCHAR *pText);
	void setStyle(UINT pStyle);

	virtual void draw(HDC pContext);
	virtual void calculateSizes(bool pSizeGiven = false);
protected:
	TCHAR text[256];
	UINT fontStyle;
};

#endif // CLSTEXTITEM_H
