#include "clsTaskItem.h"

clsTaskItem::clsTaskItem(tasklist *pTask, bool pVertical): clsItem(pVertical)
{
	fixed = DIM_NONE;
	if (pTask->active)
		style = SN_TOOLBARLABEL;
	else
		style = SN_TOOLBARBUTTON;
	taskWnd = pTask->hwnd;
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
	if (RectVisible(pContext, &itemArea))
	{
		RECT textArea = itemArea;
		textArea.left += 2;
		textArea.top += 2;
		textArea.bottom -= 2;
		textArea.right -= 2;
		MakeStyleGradient(pContext, &itemArea, bbStyle.getStyle(style), bbStyle.getStyleBorder(style));
		SetBkMode(pContext, TRANSPARENT);
		HFONT oldFont = (HFONT) SelectObject(pContext, bbStyle.getStyleFont(SN_TOOLBARCLOCK));
		COLORREF oldColor = SetTextColor(pContext, bbStyle.getStyleTextColor(style));
		DrawText(pContext, caption, -1, &textArea, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
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
	switch (msg)
	{
	case BB_TASKSUPDATE:
		switch (lParam)
		{
		case TASKITEM_MODIFIED:
			if ((HWND)wParam == taskWnd)
			{
				tasklist *task = GetTaskListPtr();
				for (int i = 0; i < GetTaskListSize(); ++i)
				{
					if (task->hwnd == (HWND)wParam)
					{

#ifdef UNICODE
						MultiByteToWideChar(CP_ACP, 0, task->caption, -1, caption, 256);
#else
						strcpy(caption, task->caption);
#endif
						InvalidateRect(barWnd, &itemArea, TRUE);
						break;
					}
					task = task->next;
				}
			}
			return 0;
		case TASKITEM_ACTIVATED:
			if (taskWnd == (HWND)wParam)
				style = SN_TOOLBARLABEL;
			else
				style = SN_TOOLBARBUTTON;
			InvalidateRect(barWnd, &itemArea, TRUE);
			return 0;
		}
		break;
	case WM_LBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_LBUTTON;
		break;
	case WM_RBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_RBUTTON;
		break;
	case WM_MBUTTONDOWN:
		mouseDown = true;
		mouseButton |= MK_MBUTTON;
		break;
	case WM_XBUTTONDOWN:
		mouseDown = true;
		switch (HIWORD(wParam))
		{
		case XBUTTON1:
			mouseButton |= MK_XBUTTON1;
			break;
		case XBUTTON2:
			mouseButton |= MK_XBUTTON2;
			break;
		}
		break;
	case WM_LBUTTONUP:
		if (mouseDown && (mouseButton & MK_LBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			OutputDebugString(TEXT("Left Click in task"));
			PostMessage(hBlackboxWnd, BB_BRINGTOFRONT, 0,  (LPARAM)taskWnd);
		}
		break;
	case WM_RBUTTONUP:
		if (mouseDown && (mouseButton & MK_RBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			OutputDebugString(TEXT("Right Click in task"));
		}
		break;
	case WM_MBUTTONUP:
		if (mouseDown && (mouseButton & MK_MBUTTON))
		{
			mouseButton = 0;
			mouseDown = false;
			OutputDebugString(TEXT("Mid Click in task"));
		}
		break;
	case WM_XBUTTONUP:
		switch (HIWORD(wParam))
		{
		case XBUTTON1:
			if (mouseDown && (mouseButton & MK_XBUTTON1))
			{
				mouseButton = 0;
				mouseDown = false;
				OutputDebugString(TEXT("X1 Click in task"));
			}
			break;
		case XBUTTON2:
			if (mouseDown && (mouseButton & MK_XBUTTON2))
			{
				mouseButton = 0;
				mouseDown = false;
				OutputDebugString(TEXT("X2 Click in task"));
				break;
			}
		}
		break;
	}
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
		minSizeX = 18;
		resize(minSizeX, minSizeY);
	}
}

/** @brief resize
  *
  * @todo: document this function
  */
dimType clsTaskItem::resize(int pX, int pY)
{
	dimType done = DIM_NONE;
	if (pX >= minSizeX)
	{
		itemArea.right = itemArea.left + pX;
		done = DIM_HORIZONTAL;
	}
	if ((pY >= minSizeY) && (pY <= 18))
	{
		itemArea.bottom = itemArea.top + pY;
		done = ((done == DIM_HORIZONTAL) ? DIM_BOTH : DIM_VERTICAL);
	}
	return done;
}

