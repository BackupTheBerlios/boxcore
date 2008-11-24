#include "clsTaskItem.h"

clsTaskItem::clsTaskItem(tasklist *pTask, bool pVertical):clsItem(pVertical)
{
	fixed = DIM_VERTICAL;
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, pTask->caption, -1, caption, 256);
#else
	strcpy(caption, pTask->caption);
#endif
}

clsTaskItem::~clsTaskItem()
{
	//dtor
}

void clsTaskItem::draw(HDC pContext)
{
	OutputDebugString(TEXT("Maybe drawing task"));
	if (RectVisible(pContext, &itemArea))
	{
		OutputDebugString(TEXT("Drawing task"));
		/// @todo fix to not fetch each time
		MakeStyleGradient(pContext, &itemArea, bbStyle.getStyle(SN_TOOLBARCLOCK), bbStyle.getStyleBorder(SN_TOOLBARCLOCK));
		SetBkMode(pContext, TRANSPARENT);
		HFONT oldFont = (HFONT) SelectObject(pContext, bbStyle.getStyleFont(SN_TOOLBARCLOCK));
		COLORREF oldColor = SetTextColor(pContext, bbStyle.getStyleTextColor(SN_TOOLBARCLOCK));
		DrawText(pContext, caption, -1, &itemArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		SetTextColor(pContext, oldColor);
		SelectObject(pContext, oldFont);
	}
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsTaskItem::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

/** @brief calculateSizes
  *
  * @todo: document this function
  */
void clsTaskItem::calculateSizes(bool pSizeGiven)
{
	if (!pSizeGiven)
	{
	minSizeY = 18;
	minSizeX = 0;
	resize(minSizeX, minSizeY);
	}
}

