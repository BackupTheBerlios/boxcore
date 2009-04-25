#ifndef CLSICONITEM_H
#define CLSICONITEM_H

#include "clsItem.h"

namespace boxBar
{

class clsIconItem : public Item
{
public:
	clsIconItem(HICON pIcon, UINT pSize);
	~clsIconItem();

	bool setIcon(HICON pIcon);

	virtual void draw(HDC pContext);
	virtual void calculateSizes(bool pSizeGiven = false);
protected:
	HICON icon;
	UINT iconSize;
private:
};

}

#endif // CLSICONITEM_H
