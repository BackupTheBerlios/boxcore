#include "clsTaskItem.h"
#include "clsTextItem.h"
#include "clsFlexiSpacer.h"
#include <limits.h>

clsTaskItem::clsTaskItem(tasklist *pTask, bool pVertical): clsItemCollection(pVertical)
{
	m_knowsSize = DIM_VERTICAL;
	m_wantsStretch = DIM_HORIZONTAL;
	vertical = false;

	taskWnd = pTask->hwnd;
	CopyString(m_caption, pTask->caption, 256);
	readSettings();
	if (iconSize > 16)
		if (GlobalFindAtom(TEXT("boxCore::running")))
			iconItem = new clsIconItem(pTask->icon_big, iconSize, vertical);
		else
			iconItem = new clsIconItem(pTask->icon, iconSize, vertical);
	else
		iconItem = new clsIconItem(pTask->icon, iconSize, vertical);
	captionItem = new clsTextItem(m_caption, style, vertical);
	addItem(iconItem);
	addItem(captionItem);
	leftClick = activateTask;

	if (pTask->active)
	{
		style = activeStyle;
		itemAlpha = activeAlpha;
	}
	else
	{
		style = inactiveStyle;
		itemAlpha = inactiveAlpha;
	}
}

/** @brief TaskItem destructer
  *
  * Cleans up the tooltip, instead of letting it get handled by the base destructor. This
  * is needed because out tipText is not dynamically allocated.
  */
clsTaskItem::~clsTaskItem()
{
	delete iconItem;
	delete captionItem;
	itemList.clear();
	tipText = NULL;
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
				tipText = NULL;
				ClearTooltip();
				for (int i = 0; i < GetTaskListSize(); ++i)
				{
					if (task->hwnd == (HWND)wParam)
					{
						CopyString(m_caption, task->caption, 256);
						captionItem->setText(m_caption);
						if (iconSize > 16)
							if (GlobalFindAtom(TEXT("boxCore::running")))
								iconItem->setIcon(task->icon_big);
							else
								iconItem->setIcon(task->icon);
						else
							iconItem->setIcon(task->icon);
						break;
					}
					task = task->next;
				}
			}
			return 0;
		case TASKITEM_ACTIVATED:
			if (taskWnd == (HWND)wParam)
			{
				style = activeStyle;
				itemAlpha = activeAlpha;
			}
			else
			{
				style = inactiveStyle;
				itemAlpha = inactiveAlpha;
			}
			captionItem->setStyle(style);
			return 0;
		}
		break;
	case BOXBAR_NEEDTIP:
		if ((clsTextItem *)lParam == captionItem)
		{
			if (wParam)
				tipText = m_caption;
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
	clsTaskItem *realItem = dynamic_cast<clsTaskItem *>(pItem);
	if (realItem)
	{
		PostMessage(hBlackboxWnd, BB_BRINGTOFRONT, 0,  (LPARAM)realItem->taskWnd);
	}
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void clsTaskItem::readSettings()
{
	iconSize = ReadInt(configFile, "boxBar.tasks.iconsize:", 16);
	spacingBorder = ReadInt(configFile, "boxBar.tasks.task.spacingBorder:", 2);
	spacingItems = ReadInt(configFile, "boxBar.tasks.task.spacingItems:", 2);
	m_maxSizeX = ReadInt(configFile, "boxBar.tasks.task.maxSize.X:", 0);
	if (m_maxSizeX <= 0)
	{
		m_maxSizeX = INT_MAX;
	}

	activeAlpha = ReadInt(configFile, "boxBar.tasks.active.alpha:", 255);
	inactiveAlpha = ReadInt(configFile, "boxBar.tasks.inactive.alpha:", 255);
	activeStyle = ReadBool(configFile, "boxBar.tasks.active.background:", true) ? SN_TOOLBARWINDOWLABEL : 0;
	inactiveStyle = ReadBool(configFile, "boxBar.tasks.inactive.background:", true) ? SN_TOOLBAR : 0;
}

/** @brief configMenu
  *
  * @todo: document this function
  */
void clsTaskItem::configMenu(Menu *pMenu, bool p_update)
{
	char ansiCaption[256];
	char command[256];
	CopyString(ansiCaption, m_caption, 256);
	sprintf(command, "%s (%llu)", ansiCaption, (UINT64)taskWnd);
	Menu *subMenu = MakeNamedMenu(command, command, !p_update);
	MakeSubmenu(pMenu, subMenu, command);

	sprintf(command, "@boxbar.task.front.%llu", (UINT64)taskWnd);
	MakeMenuItem(subMenu, "Move to start", command, false);
	sprintf(command, "@boxbar.task.left.%llu", (UINT64)taskWnd);
	MakeMenuItem(subMenu, "Move left/up", command, false);
	sprintf(command, "@boxbar.task.right.%llu", (UINT64)taskWnd);
	MakeMenuItem(subMenu, "Move right/down", command, false);
	sprintf(command, "@boxbar.task.end.%llu", (UINT64)taskWnd);
	MakeMenuItem(subMenu, "Move to end", command, false);
}

int clsTaskItem::inactiveStyle = SN_TOOLBAR;
int clsTaskItem::activeStyle = SN_TOOLBARWINDOWLABEL;
int clsTaskItem::inactiveAlpha = 255;
int clsTaskItem::activeAlpha = 255;
