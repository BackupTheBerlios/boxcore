#include "BBApi.h"
#include "clsTaskItemCollection.h"
#include "clsTaskItem.h"
#include "clsLabelItem.h"

clsTaskItemCollection::clsTaskItemCollection(bool pVertical):clsItemCollection(pVertical)
{
	//ctor

	spacingBorder = 0;
	spacingItems = 2;
	populateTasks();
}

clsTaskItemCollection::~clsTaskItemCollection()
{
	//dtor
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsTaskItemCollection::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case BB_TASKSUPDATE:
			switch(lParam)
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
	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		delete (*i);
	}
	itemList.clear();
	tasklist *task = GetTaskListPtr();
	for(int i=0; i< GetTaskListSize(); ++i)
	{
		addItem(new clsTaskItem(task, vertical));
		task = task->next;
	}
	calculateSizes(true);
	InvalidateRect(barWnd, &itemArea, TRUE);
}



