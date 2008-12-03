#include "BBApi.h"
#include "clsTaskItemCollection.h"
#include "clsTaskItem.h"
#include "clsFlexiSpacer.h"

clsTaskItemCollection::clsTaskItemCollection(bool pVertical): clsItemCollection(pVertical)
{
	spacingBorder = 0;
	spacingItems = 2;

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
	case BB_TASKSUPDATE:
		switch (lParam)
		{
		case TASKITEM_REMOVED:
		case TASKITEM_ADDED:
			populateTasks();
			break;
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
	tasklist *task = GetTaskListPtr();
	dbg_printf("Tasklist size : %d", GetTaskListSize());
	for (int i = 0; i < GetTaskListSize(); ++i)
	{
		addItem(new clsTaskItem(task, vertical));
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
	Menu *subMenu = MakeNamedMenu("boxBar.tasks", "boxBar.tasks", true);
	MakeSubmenu(pMenu, subMenu, "Tasks Configuration");
	MakeMenuItemInt(subMenu, "Icon size", "@boxBar.tasks.iconsize", ReadInt(configFile, "boxBar.tasks.iconsize:", 16), 0, 256);
	MakeMenuItem(subMenu, "Vertical", "@boxBar.vertical", vertical);
}


