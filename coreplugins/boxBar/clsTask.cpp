#include "clsTask.h"
#include "clsText.h"
#include "clsIcon.h"
#include <cstdlib>

namespace boxBar
{

/**
 * @page boxBarRC
 * @section boxBarTasks Tasks
 * The tasks item displays the list of currently running tasks, similar to the taskbar in
 * windows explorer. Each task is a @ref boxBarCollection, so those settings can be applied to the Tasks item.
 * @code boxBar.Tasks.IconSize: 32 @endcode
 * The size of the icon for a task, if enabled
 *
 * @code boxBar.Tasks.ShowIcon: true @endcode
 * Determines whether tasks display their icon, or not.
 *
 * @code boxBar.Tasks.ShowText: true @endcode
 * Determines whether tasks display the window title, or not.
 *
 * @page boxBarRCAdvanced
 * @section boxBarTasksAdv Tasks
 * Advanced settings for @ref boxBarTasks. The tasks item is also affected by @ref boxBarTaskAreaAdv
 *
 * @code boxBar.Tasks.Active.Background: true @endcode
 * Sets whether active tasks draw a background for the task.
 *
 * @code boxBar.Tasks.Inactive.Background: true @endcode
 * Sets whether inactive tasks draw a background for the task.
 */

Task::Task(HWND p_Task, bool pVertical, LPCSTR p_itemName):
		Collection(pVertical, "Tasks", 2, 2, minMaxStruct(dummyZeroInt, s_settingsManager.AssociateInt(m_pluginPrefix, p_itemName, "MaxSize.X", 0))),
		iconSize(s_settingsManager.AssociateUInt(m_pluginPrefix, p_itemName, "IconSize", 32)),
		m_activeBackground(s_settingsManager.AssociateBool(m_pluginPrefix, p_itemName, "Active.Background", true)),
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
		iconItem = new Icon(LoadIcon(NULL, IDI_APPLICATION), iconSize);
		addItem(iconItem);
	}
	else
	{
		iconItem = NULL;
	}
	if (m_showText || !m_showIcon)
	{
		captionItem = new Text(m_caption, style);
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
		if ((Item *)lParam == captionItem || (Item *)lParam == iconItem)
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
	return Item::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief activateTask
  *
  * @todo: document this function
  */
void Task::activateTask(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Task *realItem = dynamic_cast<Task *>(pItem);
	if (realItem)
	{
		PostMessage(hBlackboxWnd, BB_BRINGTOFRONT, 0,  (LPARAM)realItem->taskWnd);
	}
}

void Task::WindowMenu(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
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
	Item::readSettings();
	if (m_maxSizeX <= 0)
	{
		m_maxSizeX = INT_MAX;
	}
	activeStyle = m_activeBackground ? SN_TOOLBARWINDOWLABEL : 0;
	inactiveStyle = m_inactiveBackground ? SN_TOOLBAR : 0;
}

VOID CALLBACK Task::SmallIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult)
{
	Task *task = reinterpret_cast<Task *>(p_dwData);
	if (task->taskWnd == p_hWnd)
	{
		if (p_lResult)
		{
			task->iconItem->SetIcon(reinterpret_cast<HICON>(p_lResult));
		}
		else
		{
			if (!task->iconItem->SetIcon(reinterpret_cast<HICON>(GetClassLongPtr(task->taskWnd, GCLP_HICONSM))))
			{
				if (task->m_fallback)
				{
					task->iconItem->SetIcon(LoadIcon(NULL, IDI_APPLICATION));
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
			task->iconItem->SetIcon(reinterpret_cast<HICON>(p_lResult));
		}
		else
		{
			if (!task->iconItem->SetIcon(reinterpret_cast<HICON>(GetClassLongPtr(task->taskWnd, GCLP_HICON))))
			{
				if (task->m_fallback)
				{
					task->iconItem->SetIcon(LoadIcon(NULL, IDI_APPLICATION));
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
	sprintf(command, "%s (%Iu)", ansiCaption, (UINT_PTR)taskWnd);
	Menu *subMenu = MakeNamedMenu(command, command, !p_update);
	MakeSubmenu(pMenu, subMenu, command);

	sprintf(command, "@boxbar.task.front.%Iu", (UINT_PTR)taskWnd);
	MakeMenuItem(subMenu, "Move to start", command, false);
	sprintf(command, "@boxbar.task.left.%Iu", (UINT_PTR)taskWnd);
	MakeMenuItem(subMenu, "Move left/up", command, false);
	sprintf(command, "@boxbar.task.right.%Iu", (UINT_PTR)taskWnd);
	MakeMenuItem(subMenu, "Move right/down", command, false);
	sprintf(command, "@boxbar.task.end.%Iu", (UINT_PTR)taskWnd);
	MakeMenuItem(subMenu, "Move to end", command, false);
}

UINT Task::inactiveStyle = SN_TOOLBAR;
UINT Task::activeStyle = SN_TOOLBARWINDOWLABEL;
UINT Task::inactiveAlpha = 255;
UINT Task::activeAlpha = 255;

}

