/*
 * clsBBTask.h
 *
 *  Created on: 09 Feb 2009
 *      Author: shaun
 */

#ifndef CLSBBTASK_H_
#define CLSBBTASK_H_

#include "BBApi.h"
#include "tasks/clsLegacyTask.h"


class BBTask: public TaskManagement::LegacyTask
{
public:
	BBTask();
	virtual ~BBTask();

	virtual void Update(TaskManagement::Task *p_task);
	virtual void UpdatePrev(TaskManagement::Task *p_task);
	virtual void UpdateNext(TaskManagement::Task *p_task);

	tasklist *GetTaskList();
private:
	tasklist m_taskListNode;
};

#endif /* CLSBBTASK_H_ */
