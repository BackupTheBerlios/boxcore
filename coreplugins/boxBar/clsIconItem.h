#ifndef CLSICONITEM_H
#define CLSICONITEM_H

#include "clsItem.h"


class clsIconItem : public clsItem
{
public:
	clsIconItem(HICON pIcon, UINT pSize, bool pVertical);
	~clsIconItem();

	bool setIcon(HICON pIcon);

	virtual void draw(HDC pContext);
	virtual void calculateSizes(bool pSizeGiven = false);
protected:
	HICON icon;
	UINT iconSize;
private:
};

#endif // CLSICONITEM_H
