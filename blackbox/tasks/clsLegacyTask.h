/*
 * clsLegacyTask.h
 *
 *  Created on: 09 Feb 2009
 *      Author: shaun
 */

#ifndef CLSLEGACYTASK_H_
#define CLSLEGACYTASK_H_

#ifndef PURE
#define PURE = 0
#endif

namespace TaskManagement
{

class Task;

class LegacyTask
{
public:
	LegacyTask();
	virtual ~LegacyTask();

	virtual void Update(Task *p_task) PURE;
	virtual void UpdatePrev(Task *p_task) PURE;
	virtual void UpdateNext(Task *p_task) PURE;
};

}

#endif /* CLSLEGACYTASK_H_ */
