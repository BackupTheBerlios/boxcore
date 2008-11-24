#include "BBApi.h"
#include "clsTaskItemCollection.h"
#include "clsTaskItem.h"
#include "clsLabelItem.h"

clsTaskItemCollection::clsTaskItemCollection(bool pVertical):clsItemCollection(pVertical)
{
	//ctor
	tasklist *task = GetTaskListPtr();
	for(int i=0; i< GetTaskListSize(); ++i)
	{
		addItem(new clsTaskItem(task, pVertical));
		task = task->next;
	}
	addItem(new clsLabelItem(pVertical));
	spacingPerp = 0;
	spacingWith = 2;
}

clsTaskItemCollection::~clsTaskItemCollection()
{
	//dtor
}
