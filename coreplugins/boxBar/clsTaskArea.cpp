#include "BBApi.h"
#include "clsTaskArea.h"
#include "clsTask.h"
#include "clsFlexiSpacer.h"
#include <cstdlib>
#include <functional>
#include <algorithm>

namespace boxBar
{

TaskArea::TaskArea(bool pVertical, LPCSTR p_itemName): clsItemCollection(pVertical, p_itemName, 0, 2),
		stretchTaskarea(s_settingsManager.AssociateBool(m_pluginPrefix, m_itemPrefix, "Stretch", true)),
		m_basePrefix(p_itemName),
		m_iconSize(s_settingsManager.AssociateUInt(m_pluginPrefix, "Tasks", "IconSize", 16))
{
	m_basePrefix.resize(m_basePrefix.find_first_of("."));
	m_dragTask = NULL;
	m_dragTimer = getTimerID();
	readSettings();
	populateTasks();
	m_dropTarget = new DropTarget(this, DragAction);
	RegisterDragDrop(barWnd, m_dropTarget);

}

TaskArea::~TaskArea()
{
	RevokeDragDrop(barWnd);
	m_dropTarget->Release();
}

class TaskIsHWnd : public std::binary_function<clsItem *, HWND, bool>
{
public:
	result_type operator() (first_argument_type p_task, second_argument_type p_hWnd) const
	{
		Task *task = dynamic_cast<Task *>(p_task);
		if (task)
		{
			return (task->GetTaskWnd() == p_hWnd);
		}
		else
		{
			return false;
		}
	}
};

/** @brief wndProc
  *
  * @TODO: document this function
  */
LRESULT TaskArea::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
				msg_string += strlen(m_itemPrefix) + 1;
				if ((element = "Stretch") && !strnicmp(msg_string, element, strlen(element)))
				{
					//msg_string += strlen(element);
					//m_iconSize = atoi(msg_string);
					//s_settingsManager.WriteSetting(m_pluginPrefix, "Tasks", element);
					//populateTasks();
					//PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
			}
			else if (!strnicmp(msg_string, m_basePrefix.c_str(), m_basePrefix.size()))
			{
				if (itemList.size())
				{
					itemList.front()->wndProc(hWnd, msg, wParam, lParam);
				}
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				configMenu(NULL, true);
				return 0;
			}
		}
	}
	break;
	case BB_TASKSUPDATE:
		switch (lParam)
		{
		case TASKITEM_FLASHED:
		case TASKITEM_ACTIVATED:
			clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
			RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
			return 0;
		case TASKITEM_ADDED:

			if (std::find_if(itemList.begin(), itemList.end(), bind2nd(TaskIsHWnd(), reinterpret_cast<HWND>(wParam))) == itemList.end())
			{
				Task *newTask = new Task(reinterpret_cast<HWND>(wParam), vertical);
				itemList_t::iterator insertLoc = itemList.end();
				if (vertical && stretchTaskarea)
				{
					insertLoc--;
				}
				itemList.insert(insertLoc, newTask);
				if (stretchTaskarea)
				{
					calculateSizes(true);
					RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
				}
				else
				{
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
			}
			break;

		case TASKITEM_REMOVED:
		{
			itemList.remove_if(bind2nd(TaskIsHWnd(), reinterpret_cast<HWND>(wParam)));
			if (stretchTaskarea)
			{
				calculateSizes(true);
				RedrawWindow(barWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
			}
			else
			{
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
			}
			break;
		}
		//populateTasks();
		}
		break;
	}
	return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief populateTasks
  *
  * @TODO: document this function
  */
void TaskArea::populateTasks()
{
	lastMouse = NULL;
	for (itemList_t::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		delete (*i);
	}
	itemList.clear();
	itemMapping.clear();

	for (int i = 0; i < GetTaskListSize(); ++i)
	{
		if (GetTask(i))
		{
			Task *newTask = new Task(GetTask(i), vertical);
			addItem(newTask);
		}
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
  * it should stretch vertically or not. Detailed setting are read by Task itself
  */
void TaskArea::readSettings()
{
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
  * This adds the submenu for general task properties (as read from the RC file by TaskArea)
  * as well as specific task properties (as read by Task). The messages generated will be
  * handled separately by these two classes
  */
void TaskArea::configMenu(Menu *pMenu, bool p_update)
{
	CHAR buffer[256];
	LPCSTR menuTitle = "Tasks Configuration";
	sprintf(buffer, "%s.%s", m_pluginPrefix, m_itemPrefix);
	Menu *subMenu = MakeNamedMenu(menuTitle, buffer, !p_update);
	if (!p_update)
	{
		MakeSubmenu(pMenu, subMenu, menuTitle);
	}
	sprintf(buffer, "@%s.%s.%s", m_pluginPrefix, m_basePrefix.c_str(), "ShowIcon");
	MakeMenuItem(subMenu, "Display Icon", buffer, s_settingsManager.AssociateBool(m_pluginPrefix, m_basePrefix.c_str(), "ShowIcon", true));
	sprintf(buffer, "@%s.%s.%s", m_pluginPrefix, m_basePrefix.c_str(), "ShowText");
	MakeMenuItem(subMenu, "Display Title", buffer, s_settingsManager.AssociateBool(m_pluginPrefix, m_basePrefix.c_str(), "ShowText", true));
	sprintf(buffer, "@%s.%s.%s", m_pluginPrefix, m_basePrefix.c_str(), "IconSize");
	MakeMenuItemInt(subMenu, "Icon size", buffer, m_iconSize, 0, 256);
	sprintf(buffer, "@%s.%s.%s", m_pluginPrefix, m_basePrefix.c_str(), "Maxsize.X");
	MakeMenuItemInt(subMenu, "Maximum TaskWidth", buffer, s_settingsManager.AssociateInt(m_pluginPrefix, m_basePrefix.c_str(), "MaxSize.x", 0), 0, 1000);
	if (p_update)
	{
		ShowMenu(subMenu);
	}
}

void TaskArea::DragAction(clsItem * p_item, eDragDropState p_state, INT p_x, INT p_y)
{
	TaskArea *taskCollection = dynamic_cast<TaskArea *>(p_item);
	if (taskCollection)
	{
		switch (p_state)
		{
		case DRAG_OVER:
		{
			POINT point = {p_x, p_y};
			ScreenToClient(barWnd, &point);
			for (itemList_t::iterator i = taskCollection->itemList.begin(); i
					!= taskCollection->itemList.end(); ++i)
			{
				if ((*i)->hitTest(point.x, point.y))
				{
					if (taskCollection->m_dragTask != (*i))
					{
						SetTimer(taskCollection->barWnd, taskCollection->m_dragTimer, 500, NULL);
						taskCollection->m_dragTask = dynamic_cast<Task *>(*i);
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

}
