#include "clsTaskItem.h"
#include "clsTextItem.h"
#include "clsFlexiSpacer.h"

clsTaskItem::clsTaskItem(tasklist *pTask, bool pVertical): clsItemCollection(pVertical)
{
	fixed = DIM_VERTICAL;
	vertical = false;
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
	readSettings();
	if (iconSize > 16)
		iconItem = new clsIconItem(pTask->icon_big, iconSize, vertical);
	else
		iconItem = new clsIconItem(pTask->icon, iconSize, vertical);
	captionItem = new clsTextItem(caption, style, vertical);
	addItem(iconItem);
	addItem(captionItem);
	spacingBorder = 2;
	spacingItems = 2;
	leftClick = activateTask;
}

/** @brief TaskItem destructor
  *
  * Cleans up the tooltip, instead of letting it get handled by the base destructor. This
  * is needed because out tipText is not dynamically allocated.
  */
clsTaskItem::~clsTaskItem()
{
	if (tipText)
	{
		tipText = NULL;
		setTooltip();
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
						captionItem->setText(caption);
						if (iconSize > 16)
							iconItem->setIcon(task->icon_big);
						else
							iconItem->setIcon(task->icon);
						drawNow();
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
			captionItem->setStyle(style);
			drawNow();
			return 0;
		}
		break;
	case BOXBAR_NEEDTIP:
		if ((clsTextItem *)lParam == captionItem)
		{
			if (wParam)
				tipText = caption;
			else
				tipText = NULL;
			setTooltip();
		}
		break;
	}
	return clsItem::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief activateTask
  *
  * @todo: document this function
  */
void clsTaskItem::activateTask(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	clsTaskItem *realItem = static_cast<clsTaskItem *>(pItem);
	PostMessage(hBlackboxWnd, BB_BRINGTOFRONT, 0,  (LPARAM)realItem->taskWnd);
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void clsTaskItem::readSettings()
{
	iconSize = ReadInt(configFile, "boxBar.tasks.iconsize:", 16);
}


