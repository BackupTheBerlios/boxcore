#ifndef CLSICONITEM_H
#define CLSICONITEM_H

#include "clsItem.h"


class clsIconItem : public clsItem
{
	public:
		clsIconItem(HICON pIcon, int pSize, bool pVertical);
		virtual ~clsIconItem();

		void setIcon(HICON pIcon);

		virtual void draw(HDC pContext);
		virtual void calculateSizes(bool pSizeGiven = false);
	protected:
		HICON icon;
		int iconSize;
	private:
};

#endif // CLSICONITEM_H
