#include "BBApi.h"
#include "clsTaskItemCollection.h"
#include "clsTaskItem.h"
#include "clsFlexiSpacer.h"
#include <cstdlib>

#include "rcworker/clsRCInt.h"
#include "rcworker/clsRCBool.h"

clsTaskItemCollection::clsTaskItemCollection(bool pVertical): clsItemCollection(pVertical)
{
	CHAR buffer[256];
	m_dragTask = NULL;
	m_dragTimer = getTimerID();
	m_itemPrefix = new CHAR[strlen("Tasks")+1];
	strcpy(m_itemPrefix, "Tasks");
	m_workers.push_back(new RCWorkers::RCBool(configFile, ItemRCKey(buffer, "stretcharea"), stretchTaskarea, true));
	m_workers.push_back(new RCWorkers::RCInt(configFile, ItemRCKey(buffer,"SpacingBorder"), spacingBorder, 0));
	m_workers.push_back(new RCWorkers::RCInt(configFile, ItemRCKey(buffer,"SpacingItems"), spacingItems, 2));
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
			}
			KillTimer(barWnd, m_dragTimer);
		}
		break;
	case BB_RECONFIGURE:
		readSettings();
		populateTasks();
		break;
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
				CHAR buffer[256];
				msg_string += strlen(m_itemPrefix) + 1;
				if ((element = "iconSize") && !strnicmp(msg_string, element, strlen(element)))
				{
					msg_string += strlen(element);
					WriteInt(configFile, ItemRCKey(buffer, element),atoi(msg_string));
					readSettings();
					populateTasks();
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
				else if ((element = "MaxSize.X") && !strnicmp(msg_string, element, strlen(element)))
				{
					msg_string += strlen(element);
					WriteInt(configFile, ItemRCKey(buffer, element),atoi(msg_string));
					readSettings();
					populateTasks();
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
			}
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
			populateTasks();
			break;
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
	while (task)
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
	clsItem::readSettings();

	if (stretchTaskarea)
	{
		m_wantsStretch = DIM_BOTH;
		if (vertical)
		{
			m_knowsSize = DIM_NONE;
		}
		else
		{
			m_knowsSize = DIM_VERTICAL;
		}
	}
	else
	{
		m_wantsStretch = DIM_NONE;
		m_knowsSize = DIM_VERTICAL;
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
	CHAR buffer[256];
	LPCSTR menuTitle = "Tasks Configuration";
	sprintf(buffer, "%s.%s", m_pluginPrefix, m_itemPrefix);
	Menu *subMenu = MakeNamedMenu(menuTitle, buffer, !p_update);
	if (!p_update)
	{
		MakeSubmenu(pMenu, subMenu, menuTitle);
	}
	MakeMenuItemInt(subMenu, "Icon size", ItemBroam(buffer, "iconSize"), ReadInt(configFile, ItemRCKey(buffer,"iconSize"), 16), 0, 256);
	MakeMenuItemInt(subMenu, "Maximum TaskWidth", ItemBroam(buffer, "maxSize.X"), ReadInt(configFile, ItemRCKey(buffer,"maxSize.X"), 0), 0, 1000);
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




