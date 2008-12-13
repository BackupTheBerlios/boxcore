#include "BBApi.h"
#include "clsTaskItemCollection.h"
#include "clsTaskItem.h"
#include "clsFlexiSpacer.h"

clsTaskItemCollection::clsTaskItemCollection(bool pVertical): clsItemCollection(pVertical)
{
	readSettings();
	populateTasks();
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsTaskItemCollection::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case BB_RECONFIGURE:
		readSettings();
		populateTasks();
		break;
	case BB_BROADCAST:
		{
			dbg_printf((LPCSTR) lParam);
			LPCSTR msg_string = (LPCSTR)lParam;
			if (!strnicmp(msg_string, "@boxBar.task.front.", strlen("@boxBar.task.front.")))
			{
				HWND targetTask;
				sscanf(msg_string + strlen("@boxBar.task.front."), "%llu", (UINT64 *)&targetTask);
				list<clsItem *>::iterator targetIt;
				for(targetIt = itemList.begin(); targetIt != itemList.end(); ++ targetIt)
				{
					if (static_cast<clsTaskItem *>(*targetIt)->GetTaskWnd() == targetTask)
						break;
				}
				if (targetIt != itemList.end())
				{
					clsTaskItem *targetItem = static_cast<clsTaskItem *>(*targetIt);
					itemList.erase(targetIt);
					itemList.push_front(targetItem);
				}
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
			}
			else if (!strnicmp(msg_string, "@boxBar.task.end.", strlen("@boxBar.task.end.")))
			{
				HWND targetTask;
				sscanf(msg_string + strlen("@boxBar.task.end."), "%llu", (UINT64 *)&targetTask);
				list<clsItem *>::iterator targetIt;
				for(targetIt = itemList.begin(); targetIt != itemList.end(); ++ targetIt)
				{
					if (static_cast<clsTaskItem *>(*targetIt)->GetTaskWnd() == targetTask)
						break;
				}
				if (targetIt != itemList.end())
				{
					clsTaskItem *targetItem = static_cast<clsTaskItem *>(*targetIt);
					itemList.erase(targetIt);
					itemList.push_back(targetItem);
				}
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
			}
			else if (!strnicmp(msg_string, "@boxBar.task.left.", strlen("@boxBar.task.left.")))
			{
				HWND targetTask;
				sscanf(msg_string + strlen("@boxBar.task.left."), "%llu", (UINT64 *)&targetTask);
				list<clsItem *>::iterator targetIt;
				for(targetIt = itemList.begin(); targetIt != itemList.end(); ++ targetIt)
				{
					if (static_cast<clsTaskItem *>(*targetIt)->GetTaskWnd() == targetTask)
						break;
				}
				if (targetIt != itemList.end())
				{
					list<clsItem *>::iterator prevIt = targetIt;
					prevIt--;
					clsTaskItem *targetItem = static_cast<clsTaskItem *>(*targetIt);
					itemList.erase(targetIt);
					itemList.insert(prevIt, targetItem);
				}
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
			}
			else if (!strnicmp(msg_string, "@boxBar.task.right.", strlen("@boxBar.task.right.")))
			{
				HWND targetTask;
				sscanf(msg_string + strlen("@boxBar.task.right."), "%llu", (UINT64 *)&targetTask);
				list<clsItem *>::iterator targetIt;
				for(targetIt = itemList.begin(); targetIt != itemList.end(); ++ targetIt)
				{
					if (static_cast<clsTaskItem *>(*targetIt)->GetTaskWnd() == targetTask)
						break;
				}
				if (targetIt != itemList.end())
				{
					list<clsItem *>::iterator nextIt = targetIt;
					nextIt++;
					nextIt++;
					clsTaskItem *targetItem = static_cast<clsTaskItem *>(*targetIt);
					itemList.erase(targetIt);
					itemList.insert(nextIt, targetItem);
				}
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
			}
		}
		break;
	case BB_TASKSUPDATE:
		switch (lParam)
		{
		case TASKITEM_ACTIVATED:
		case TASKITEM_MODIFIED:
			clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
			InvalidateRect(barWnd, &itemArea, TRUE);
			PostMessage(barWnd, BOXBAR_REDRAW, 0, 0);
			break;
		case TASKITEM_ADDED:
			{
				tasklist *task = GetTaskListPtr();
				for (int i = 0; i < GetTaskListSize(); ++i)
				{
					if (task->hwnd == (HWND)wParam)
					{
						clsTaskItem *newTask = new clsTaskItem(task, vertical);
						addItem(newTask);
						itemMapping[task->hwnd] = newTask;
						break;
					}
					task = task->next;
				}
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
			}
			return 0;
		case TASKITEM_REMOVED:
			{
				clsItem *removedTask;
				removedTask = itemMapping[(HWND)wParam];
				for (list<clsItem *>::iterator i = itemList.begin(); i != itemList.end(); ++i)
				{
					if ((*i) == removedTask)
					{
						itemList.erase(i);
						break;
					}
				}
				itemMapping.erase((HWND)wParam);
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE);
			}
			return 0;
		}
		break;
	}
	return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief populateTasks
  *
  * @todo: document this function
  */
void clsTaskItemCollection::populateTasks()
{
	lastMouse = NULL;
	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		delete (*i);
	}
	itemList.clear();
	itemMapping.clear();

	tasklist *task = GetTaskListPtr();
	for (int i = 0; i < GetTaskListSize(); ++i)
	{
		clsTaskItem *newTask = new clsTaskItem(task, vertical);
		addItem(newTask);
		itemMapping[task->hwnd] = newTask;
		task = task->next;
	}
	if (vertical && stretchTaskarea)
		addItem(new clsFlexiSpacer(vertical));
	calculateSizes(true);
	if (!stretchTaskarea)
		SendMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
	else
		InvalidateRect(barWnd, &itemArea, TRUE);
}


/** @brief Reads task area settings from RC file
  *
  * This function reads settings related to the general task area, such as whether
  * it should stretch verticcally or not. Detailed setting are read by clsTaskItem itself
  */
void clsTaskItemCollection::readSettings()
{
	stretchTaskarea = ReadBool(configFile, "boxBar.tasks.stretcharea:", true);
	spacingBorder = ReadInt(configFile, "boxBar.tasks.spacingBorder:", 0);
	spacingItems = ReadInt(configFile, "boxBar.tasks.spacingItems:", 2);

	if (stretchTaskarea)
		fixed = (vertical ? DIM_HORIZONTAL : DIM_VERTICAL);
	else
		fixed = DIM_BOTH;
}

/** @brief Add submenu for tasks configuration
  *
  * @param[in,out] pMenu Menu pointer to which items can be added
  *
  * This adds the submenu for general task properties (as read from the RC file by clsTaskItemCollection)
  * as well as specific task properties (as read by clsTaskItem). The messages generated will be
  * handled seperately by these two classes
  */
void clsTaskItemCollection::configMenu(Menu *pMenu)
{
	Menu *subMenu = MakeNamedMenu("Tasks Configuration","boxBar.tasks", true);
	MakeSubmenu(pMenu, subMenu, "Tasks Configuration");
	MakeMenuItemInt(subMenu, "Icon size", "@boxBar.tasks.iconsize", ReadInt(configFile, "boxBar.tasks.iconsize:", 16), 0, 256);
	subMenu = MakeNamedMenu("Task Ordering","boxBar.taskorder", true);
	MakeSubmenu(pMenu, subMenu, "Task Ordering");
	clsItemCollection::configMenu(subMenu);
}


