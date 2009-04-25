#ifndef CLSICONITEM_H
#define CLSICONITEM_H

#include "clsItem.h"

namespace boxBar
{

class Icon : public Item
{
public:
	Icon(HICON pIcon, UINT pSize);
	~Icon();

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
