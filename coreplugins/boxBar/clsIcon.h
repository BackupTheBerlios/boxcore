#ifndef CLSICONITEM_H
#define CLSICONITEM_H

#include "clsItem.h"

namespace Gdiplus
{
class Bitmap;
}

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
	INT m_iconSize;

	HBITMAP m_alphaBitmap;
	HBITMAP m_bufferBitmap;
	BYTE *m_alphaBits;
	BYTE *m_bufferBits;
	Gdiplus::Bitmap *m_bitmap;

	bool m_bufferValid;
};

}

#endif // CLSICONITEM_H
