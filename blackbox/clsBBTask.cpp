/*
 * clsBBTask.cpp
 *
 *  Created on: 09 Feb 2009
 *      Author: shaun
 */

#include "clsBBTask.h"
#include "tasks/clsTask.h"
#include "../utility/stringcopy.h"
#include "managers.h"

BBTask::BBTask()
{
	m_taskListNode.next = NULL;
}

BBTask::~BBTask()
{
}

void BBTask::Update(TaskManagement::Task *p_task)
{
	m_taskListNode.hwnd = p_task->getHWnd();
	m_taskListNode.icon = p_task->getSmallIcon();
	m_taskListNode.icon_big = p_task->getLargeIcon();
	m_taskListNode.wkspc = g_pVirtualWindowManager->GetWindowWorkspace(m_taskListNode.hwnd);
	CopyString(m_taskListNode.caption, p_task->getCaption(), 243);
	m_taskListNode.active = p_task->getActive();
	m_taskListNode.flashing = p_task->getFlashing();
}

void BBTask::UpdatePrev(TaskManagement::Task *p_task)
{
}

void BBTask::UpdateNext(TaskManagement::Task *p_task)
{
	if (p_task)
	{
		m_taskListNode.next = static_cast<BBTask *>(p_task->getLegacy())->GetTaskList();
	}
	else
	{
		m_taskListNode.next = NULL;
	}
}

tasklist *BBTask::GetTaskList()
{
	return &m_taskListNode;
}


