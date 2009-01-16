#include "clsIconItem.h"

clsIconItem::clsIconItem(HICON pIcon, UINT pSize, bool pVertical):clsItem(pVertical)
{
	icon = pIcon;
	iconSize = pSize;
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsIconItem::calculateSizes(bool pSizeGiven)
{
	minSizeX = iconSize;
	minSizeY = iconSize;
	resize(minSizeX, minSizeY);
}

/** @brief draw
  *
  * @todo: document this function
  */
void clsIconItem::draw(HDC pContext)
{
	if (icon)
	{
		HDC internalDC;
		HBITMAP alphaBitmap, oldBitmap, maskBitmap;
		BYTE *ptPixels, *ptPixelsMask;
		//alphaDraw = false;
		if (alphaDraw)
		{
			internalDC = CreateCompatibleDC(pContext);
			BITMAPINFO bufferInfo;
			ZeroMemory(&bufferInfo, sizeof(bufferInfo));
			bufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bufferInfo.bmiHeader.biWidth = itemArea.right - itemArea.left;
			bufferInfo.bmiHeader.biHeight = itemArea.bottom - itemArea.top;
			bufferInfo.bmiHeader.biPlanes = 1;
			bufferInfo.bmiHeader.biBitCount = 32;
			alphaBitmap = CreateDIBSection(pContext, &bufferInfo, DIB_RGB_COLORS, (void **) & ptPixels, NULL, 0);
			maskBitmap = CreateDIBSection(pContext, &bufferInfo, DIB_RGB_COLORS, (void **) & ptPixelsMask, NULL, 0);
			oldBitmap = (HBITMAP) SelectObject(internalDC, maskBitmap);
			DrawIconEx(internalDC, 0, 0, icon, iconSize, iconSize, NULL, NULL, DI_MASK);
			SelectObject(internalDC, alphaBitmap);
			DrawIconEx(internalDC, 0, 0, icon, iconSize, iconSize, NULL, NULL, DI_NORMAL);
			for (int i = 0; i < (itemArea.bottom - itemArea.top)*(itemArea.right - itemArea.left); ++i)
			{
				if (ptPixels[3] == 0)
					ptPixels[3] = 255-ptPixelsMask[0];
				ptPixels+=4;
				ptPixelsMask+=4;
			}
			BLENDFUNCTION blendFunc;
			blendFunc.BlendOp = AC_SRC_OVER;
			blendFunc.BlendFlags = 0;
			blendFunc.SourceConstantAlpha = itemAlpha;
			blendFunc.AlphaFormat = AC_SRC_ALPHA;
			msimg32.AlphaBlend(pContext, itemArea.left, itemArea.top, itemArea.right - itemArea.left, itemArea.bottom - itemArea.top, internalDC,
							   0, 0, itemArea.right - itemArea.left, itemArea.bottom - itemArea.top, blendFunc);
			SelectObject(internalDC, oldBitmap);
			DeleteObject(alphaBitmap);
			DeleteObject(maskBitmap);
			DeleteDC(internalDC);
		}
		else
		{
			DrawIconEx(pContext, itemArea.left, itemArea.top, icon, iconSize, iconSize, NULL, NULL, DI_NORMAL);
		}
	}
}

/** @brief setIcon
  *
  * @todo: document this function
  */
void clsIconItem::setIcon(HICON pIcon)
{
	icon = pIcon;
	if (icon)
	{
		InvalidateRect(barWnd, &itemArea, TRUE);
		PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
	}

}


