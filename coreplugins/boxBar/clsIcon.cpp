#include "clsIcon.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#include <GdiPlus.h>
#undef min
#undef max

namespace boxBar
{

Icon::Icon(HICON p_icon, UINT p_iconSize):
		Item("GenericIcon"),
		m_icon(CopyIcon(p_icon)),
		m_iconSize(p_iconSize),
		m_bufferValid(false)
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;

	BITMAPINFO bufferInfo;
	ZeroMemory(&bufferInfo, sizeof(bufferInfo));
	bufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bufferInfo.bmiHeader.biWidth = m_iconSize;
	bufferInfo.bmiHeader.biHeight = m_iconSize;
	bufferInfo.bmiHeader.biPlanes = 1;
	bufferInfo.bmiHeader.biBitCount = 32;

	HDC tempDC = CreateCompatibleDC(NULL);
	m_alphaBitmap = CreateDIBSection(tempDC, &bufferInfo, DIB_RGB_COLORS, (void **) &m_alphaBits, NULL, 0);
	m_bufferBitmap = CreateDIBSection(tempDC, &bufferInfo, DIB_RGB_COLORS, (void **) &m_bufferBits, NULL, 0);
	m_bitmap = NULL;
	DeleteDC(tempDC);
}

Icon::~Icon()
{
	delete m_bitmap;
	DeleteObject(m_alphaBitmap);
	DeleteObject(m_bufferBitmap);
	if (m_icon)
	{
		DestroyIcon(m_icon);
	}
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void Icon::calculateSizes(bool p_sizeGiven)
{
	minSizeX = m_iconSize;
	minSizeY = m_iconSize;
	resize(m_iconSize, m_iconSize);
}

/** @brief draw
  *
  * @todo: document this function
  */
void Icon::draw(HDC p_context)
{
	if (m_icon)
	{
		if (!m_bufferValid)
		{
			ZeroMemory(m_bufferBits, m_iconSize*m_iconSize*4);
			ZeroMemory(m_alphaBits, m_iconSize*m_iconSize*4);
			delete m_bitmap;
			m_bitmap = NULL;
			HDC internalDC = CreateCompatibleDC(p_context);
			HBITMAP oldBitmap = (HBITMAP) SelectObject(internalDC, m_alphaBitmap);
			DrawIconEx(internalDC, 0, 0, m_icon, m_iconSize, m_iconSize, NULL, NULL, DI_MASK);
			SelectObject(internalDC, m_bufferBitmap);
			DrawIconEx(internalDC, 0, 0, m_icon, m_iconSize, m_iconSize, NULL, NULL, DI_NORMAL);
			for (BYTE *pixels = m_bufferBits + 3, *mask = m_alphaBits; pixels < m_bufferBits + m_iconSize*m_iconSize*4; pixels+=4, mask+= 4)
			{
				if (*pixels == 0)
				{
					*pixels = 255 - *mask;
				}
			}
			SelectObject(internalDC, oldBitmap);
			DeleteDC(internalDC);
			m_bitmap = new Gdiplus::Bitmap(m_iconSize, m_iconSize, m_iconSize * 4, PixelFormat32bppPARGB, m_bufferBits);
			m_bitmap->RotateFlip(Gdiplus::RotateNoneFlipY);
			m_bufferValid = true;
		}
		Gdiplus::Graphics canvas(p_context);
		canvas.DrawImage(m_bitmap, itemArea.left, itemArea.top, m_iconSize, m_iconSize);
	}
}


/**
 * @brief Sets the icon to display
 *
 * @param p_icon The new icon to display
 * @return Returns true if the icon was successfully set
 *
 * This function creates a copy of the icon passed in and stores it.
 * It does not trigger an automatic redraw.
 */
bool Icon::SetIcon(HICON p_icon)
{
	m_bufferValid = false;
	if (m_icon)
	{
		DestroyIcon(m_icon);
	}
	m_icon = CopyIcon(p_icon);
	if (m_icon)
	{
		return true;
	}
	else
	{
		return false;
	}
}

}
