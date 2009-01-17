#include "BBApi.h"
#include "clsTaskItemCollection.h"
#include "clsTaskItem.h"
#include "clsFlexiSpacer.h"
#include <cstdlib>

clsTaskItemCollection::clsTaskItemCollection(bool pVertical): clsItemCollection(pVertical)
{
	m_dragTask = NULL;
	m_dragTimer = getTimerID();
	readSettings();
	populateTasks();
	m_dropTarget = new DropTarget(this, DragAction);
	RegisterDragDrop(barWnd, m_dropTarget);

}

clsTaskItemCollection::~clsTaskItemCollection()
{
	RevokeDragDrop(barWnd);
	m_dropTarget->Release();
}

/** @brief wndProc
  *
  * @TODO: document this function
  */
LRESULT clsTaskItemCollection::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_TIMER:
		if (wParam == m_dragTimer)
		{
			if (m_dragTask)
			{
				SendMessage(hBlackboxWnd, BB_BRINGTOFRONT, 0,  (LPARAM)m_dragTask->GetTaskWnd());
				m_dragTask = NULL;
				KillTimer(barWnd, m_dragTimer);
			}
		}
	case BB_RECONFIGURE:
		readSettings();
		populateTasks();
		break;
	case BB_BROADCAST:
	{
		LPCSTR msg_string = (LPCSTR)lParam;
		if (!strnicmp(msg_string, "@boxBar.", strlen("@boxBar.")))
		{
			msg_string += strlen("@boxBar.");
			if (!strnicmp(msg_string, "tasks.", strlen("tasks.")))
			{
				msg_string += strlen("tasks.");
			}
			else if (!strnicmp(msg_string, "task.", strlen("task.")))
			{
				msg_string += strlen("task.");
			}
		}
		if (!strnicmp(msg_string, "@boxBar.tasks.iconSize", strlen("@boxBar.tasks.iconSize")))
		{
			msg_string += strlen("@boxBar.tasks.iconSize");
			WriteInt(configFile, "boxBar.tasks.iconSize:",atoi(msg_string));
			readSettings();
			populateTasks();
			SendMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
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
			return 0;
		case TASKITEM_ADDED:
		case TASKITEM_REMOVED:
			populateTasks();
			break;
		}
		break;
	}
	return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief populateTasks
  *
  * @TODO: document this function
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
  * it should stretch vertically or not. Detailed setting are read by clsTaskItem itself
  */
void clsTaskItemCollection::readSettings()
{
	stretchTaskarea = ReadBool(configFile, "boxBar.tasks.stretcharea:", true);
	spacingBorder = ReadInt(configFile, "boxBar.tasks.spacingBorder:", 0);
	spacingItems = ReadInt(configFile, "boxBar.tasks.spacingItems:", 2);

	m_knowsSize = DIM_VERTICAL;
	if (stretchTaskarea)
	{
		m_wantsStretch = DIM_BOTH;
	}
	else
	{
		m_wantsStretch = DIM_NONE;
	}
}

/** @brief Add submenu for tasks configuration
  *
  * @param[in,out] pMenu Menu pointer to which items can be added
  *
  * This adds the submenu for general task properties (as read from the RC file by clsTaskItemCollection)
  * as well as specific task properties (as read by clsTaskItem). The messages generated will be
  * handled separately by these two classes
  */
void clsTaskItemCollection::configMenu(Menu *pMenu, bool p_update)
{
	Menu *subMenu = MakeNamedMenu("Tasks Configuration","boxBar.tasks", !p_update);
	if (!p_update)
	{
		MakeSubmenu(pMenu, subMenu, "Tasks Configuration");
	}
	MakeMenuItemInt(subMenu, "Icon size", "@boxBar.tasks.iconsize", ReadInt(configFile, "boxBar.tasks.iconsize:", 16), 0, 256);
	MakeMenuItemInt(subMenu, "Maximum TaskWidth", "@boxBar.tasks.task.maxsize.x", ReadInt(configFile, "boxBar.tasks.task.maxsize.x:", 16), 0, 1000);
	if (p_update)
	{
		ShowMenu(subMenu);
	}
}

void clsTaskItemCollection::DragAction(clsItem * p_item, eDragDropState p_state, INT p_x, INT p_y)
{
	clsTaskItemCollection *taskCollection = dynamic_cast<clsTaskItemCollection *>(p_item);
	if (taskCollection)
	{
		switch (p_state)
		{
		case DRAG_OVER:
		{
			POINT point = {p_x, p_y};
			ScreenToClient(barWnd, &point);
			for (list<clsItem *>::iterator i = taskCollection->itemList.begin(); i
					!= taskCollection->itemList.end(); ++i)
			{
				if ((*i)->hitTest(point.x, point.y))
				{
					if (taskCollection->m_dragTask != (*i))
					{
						SetTimer(taskCollection->barWnd, taskCollection->m_dragTimer, 500, NULL);
						taskCollection->m_dragTask = dynamic_cast<clsTaskItem *>(*i);
					}
				}
			}
			break;
		}
		case DRAG_ENTER:
		case DRAG_LEAVE:
		case DRAG_DROP:
			KillTimer(barWnd, taskCollection->m_dragTimer);
			taskCollection->m_dragTask = NULL;
			break;
		}
	}
}




