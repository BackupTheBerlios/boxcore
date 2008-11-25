#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsItem.h"
#include <tchar.h>

class clsTextItem : public clsItem
{
	public:
		clsTextItem(TCHAR *pText, UINT pStyle, bool pVertical);
		virtual ~clsTextItem();

		void setText(TCHAR *pText);
		void setStyle(UINT pStyle);

		virtual void draw(HDC pContext);
		virtual void calculateSizes(bool pSizeGiven = false);
	protected:
		TCHAR text[256];
		UINT fontStyle;
	private:

};

#endif // CLSTEXTITEM_H
