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

	bool SetIcon(HICON pIcon);

	virtual void draw(HDC pContext);
	virtual void calculateSizes(bool pSizeGiven = false);
private:
	HICON m_icon;
	UINT m_iconSize;

	HBITMAP m_alphaBitmap;
	HBITMAP m_bufferBitmap;
	BYTE *m_alphaBits;
	BYTE *m_bufferBits;
};

}

#endif // CLSICONITEM_H
