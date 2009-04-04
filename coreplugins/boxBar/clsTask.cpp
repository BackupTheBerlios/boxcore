#include "clsTask.h"
#include "clsTextItem.h"
#include <limits.h>
#include <cstdlib>

namespace Plugin_boxBar
{

Task::Task(HWND p_Task, bool pVertical, LPCSTR p_itemName): clsItemCollection(pVertical, "Tasks", 2, 2, s_settingsManager.AssociateInt(m_pluginPrefix, p_itemName, "MaxSize.X", 0)),
		iconSize(s_settingsManager.AssociateUInt(m_pluginPrefix, p_itemName, "IconSize", 16)),
		m_inactiveBackground(s_settingsManager.AssociateBool(m_pluginPrefix, p_itemName, "Inactive.Background", true)),
		m_showIcon(s_settingsManager.AssociateBool(m_pluginPrefix, p_itemName, "ShowIcon", true)),
		m_showText(s_settingsManager.AssociateBool(m_pluginPrefix, p_itemName, "ShowText", true))
{
	m_knowsSize = DIM_VERTICAL;
	m_wantsStretch = DIM_HORIZONTAL;
	vertical = false;
	m_fallback = false;

	taskWnd = p_Task;
	GetWindowText(p_Task, m_caption, 256);
	//m_workers.push_back(new RCWorkers::RCInt(configFile, ItemRCKey(buffer,"active.alpha"), activeAlpha, 255));
	//m_workers.push_back(new RCWorkers::RCInt(configFile, ItemRCKey(buffer,"inactive.alpha"), inactiveAlpha, 255));
	//m_workers.push_back(new RCWorkers::RCBool(configFile, ItemRCKey(buffer, "active.background"), s_activeBackground, true));
	//m_workers.push_back(new RCWorkers::RCBool(configFile, ItemRCKey(buffer, "inactive.background"), s_inactiveBackground, true));
	readSettings();
	if (GetTask(GetActiveTask()) == p_Task)
	{
		style = activeStyle;
		itemAlpha = activeAlpha;
	}
	else
	{
		style = inactiveStyle;
		itemAlpha = inactiveAlpha;
	}
	if (m_showIcon)
	{
		iconItem = new clsIconItem(LoadIcon(NULL, IDI_APPLICATION), iconSize, vertical);
		addItem(iconItem);
	}
	else
	{
		iconItem = NULL;
	}
	if (m_showText || !m_showIcon)
	{
		captionItem = new clsTextItem(m_caption, style, vertical);
		addItem(captionItem);
	}
	else
	{
		captionItem = NULL;
		m_knowsSize = DIM_BOTH;
		m_wantsStretch = DIM_NONE;
		PostMessage(barWnd, BOXBAR_NEEDTIP, (WPARAM)m_caption, (LPARAM)iconItem);
	}
	if (m_showIcon)
	{
		if (iconSize > 16)
			SendMessageCallback(taskWnd, WM_GETICON, ICON_BIG, 0, LargeIconProc, reinterpret_cast<ULONG_PTR>(this));
		else
			SendMessageCallback(taskWnd, WM_GETICON, ICON_SMALL, 0, SmallIconProc, reinterpret_cast<ULONG_PTR>(this));
	}
	leftClick = activateTask;
	rightClick = WindowMenu;
}

/** @brief TaskItem destructer
  *
  * Cleans up the tooltip, instead of letting it get handled by the base destructor. This
  * is needed because out tipText is not dynamically allocated.
  */
Task::~Task()
{
	delete iconItem;
	delete captionItem;
	iconItem = NULL;
	itemList.clear();
	tipText = NULL;
}



/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT Task::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case BB_BROADCAST:
	{
		LPCSTR msg_string = (LPCSTR)lParam;
		LPCSTR element = NULL;
		msg_string += 1;
		if (!strnicmp(msg_string, m_pluginPrefix, strlen(m_pluginPrefix)))
		{
			msg_string += strlen(m_pluginPrefix) + 1;
			if (!strnicmp(msg_string, m_itemPrefix, strlen(m_itemPrefix)))
			{
				msg_string += strlen(m_itemPrefix) + 1;
				if ((element = "IconSize") && !strnicmp(msg_string, element, strlen(element)))
				{
					msg_string += strlen(element);
					iconSize = atoi(msg_string);
					s_settingsManager.WriteSetting(m_pluginPrefix, m_itemPrefix, element);
				}
				else if ((element = "MaxSize.X") && !strnicmp(msg_string, element, strlen(element)))
				{
					msg_string += strlen(element);
					m_maxSizeX = atoi(msg_string);
					s_settingsManager.WriteSetting(m_pluginPrefix, m_itemPrefix, element);
				}
				else if ((element = "ShowIcon") && !strnicmp(msg_string, element, strlen(element)))
				{
					m_showIcon = !m_showIcon;
					s_settingsManager.WriteSetting(m_pluginPrefix, m_itemPrefix, element);
				}
				else if ((element = "ShowText") && !strnicmp(msg_string, element, strlen(element)))
				{
					m_showText = !m_showText;
					s_settingsManager.WriteSetting(m_pluginPrefix, m_itemPrefix, element);
				}
			}
		}
	}
	case BB_TASKSUPDATE:
		switch (lParam)
		{
		case TASKITEM_MODIFIED:
			if ((HWND)wParam == taskWnd)
			{
				PRINT(TEXT("Task modified"));
				GetWindowText(taskWnd, m_caption, 256);
				if (captionItem)
				{
					captionItem->setText(m_caption);
				}
				else
				{
					PostMessage(barWnd, BOXBAR_NEEDTIP, (WPARAM)m_caption, (LPARAM)iconItem);
				}

				if (m_showIcon)
				{
					m_fallback = false;
					if (iconSize > 16)
						SendMessageCallback(taskWnd, WM_GETICON, ICON_BIG, 0, LargeIconProc, reinterpret_cast<ULONG_PTR>(this));
					else
						SendMessageCallback(taskWnd, WM_GETICON, ICON_SMALL, 0, SmallIconProc, reinterpret_cast<ULONG_PTR>(this));
				}
				break;

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
			if (captionItem)
			{
				captionItem->setStyle(style);
			}
			return 0;
		case TASKITEM_FLASHED:
			if (taskWnd == (HWND)wParam)
			{
				PRINT(TEXT("Task flashed"));
				if (style == activeStyle)
				{
					style = inactiveStyle;
					itemAlpha = inactiveAlpha;
				}
				else
				{
					style = activeStyle;
					itemAlpha = activeAlpha;
				}
				if (captionItem)
				{
					captionItem->setStyle(style);
				}
			}
			return 0;
		}
		break;
	case BOXBAR_NEEDTIP:
		if ((clsItem *)lParam == captionItem || (clsItem *)lParam == iconItem)
		{
			if (wParam)
				tipText = m_caption;
			if (!m_hasTooltip)
			{
				setTooltip();
			}
		}
		break;
	}
	return clsItem::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief activateTask
  *
  * @todo: document this function
  */
void Task::activateTask(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Task *realItem = dynamic_cast<Task *>(pItem);
	if (realItem)
	{
		PostMessage(hBlackboxWnd, BB_BRINGTOFRONT, 0,  (LPARAM)realItem->taskWnd);
	}
}

void Task::WindowMenu(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Task *realItem = dynamic_cast<Task *>(pItem);
	if (realItem)
	{
		//HMENU sysMenu = GetSystemMenu(realItem->taskWnd, FALSE);
		//SendMessage(realItem->taskWnd, WM_INITMENU, reinterpret_cast<WPARAM>(sysMenu), 0);
		//TrackPopupMenu(sysMenu, 0, 100, 100, 0, realItem->barWnd, NULL);
		SendMessage(realItem->taskWnd, WM_SYSCOMMAND, SC_MOUSEMENU, 0);
	}
}

/** @brief readSettings
  *
  * @todo: document this function
  */
void Task::readSettings()
{
	clsItem::readSettings();
	if (m_maxSizeX <= 0)
	{
		m_maxSizeX = INT_MAX;
	}
	activeStyle = s_activeBackground ? SN_TOOLBARWINDOWLABEL : 0;
	inactiveStyle = m_inactiveBackground ? SN_TOOLBAR : 0;
}

VOID CALLBACK Task::SmallIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult)
{
	Task *task = reinterpret_cast<Task *>(p_dwData);
	if (task->taskWnd == p_hWnd)
	{
		if (p_lResult)
		{
			task->iconItem->setIcon(reinterpret_cast<HICON>(p_lResult));
		}
		else
		{
			if (!task->iconItem->setIcon(reinterpret_cast<HICON>(GetClassLongPtr(task->taskWnd, GCLP_HICONSM))))
			{
				if (task->m_fallback)
				{
					task->iconItem->setIcon(LoadIcon(NULL, IDI_APPLICATION));
				}
				else
				{
					task->m_fallback = true;
					SendMessageCallback(task->taskWnd, WM_GETICON, ICON_BIG, 0, LargeIconProc, reinterpret_cast<ULONG_PTR>(task));
					return;
				}
			}
		}
		RedrawWindow(task->barWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
	}
}

VOID CALLBACK Task::LargeIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult)
{
	Task *task = reinterpret_cast<Task *>(p_dwData);
	if (task->taskWnd == p_hWnd)
	{
		if (p_lResult)
		{
			task->iconItem->setIcon(reinterpret_cast<HICON>(p_lResult));
		}
		else
		{
			if (!task->iconItem->setIcon(reinterpret_cast<HICON>(GetClassLongPtr(task->taskWnd, GCLP_HICON))))
			{
				if (task->m_fallback)
				{
					task->iconItem->setIcon(LoadIcon(NULL, IDI_APPLICATION));
				}
				else
				{
					task->m_fallback = true;
					SendMessageCallback(task->taskWnd, WM_GETICON, ICON_SMALL, 0, SmallIconProc, reinterpret_cast<ULONG_PTR>(task));
					return;
				}
			}
		}
		RedrawWindow(task->barWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
	}
}

/** @brief configMenu
  *
  * @todo: document this function
  */
void Task::configMenu(Menu *pMenu, bool p_update)
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

bool Task::s_activeBackground = true;
int Task::inactiveStyle = SN_TOOLBAR;
int Task::activeStyle = SN_TOOLBARWINDOWLABEL;
int Task::inactiveAlpha = 255;
int Task::activeAlpha = 255;

}

