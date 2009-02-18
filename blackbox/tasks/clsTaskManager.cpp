/*
 * clsTaskManager.cpp
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#include "clsTaskManager.h"
#include "clsTask.h"
#include "clsLegacyTask.h"
#include "helpers.h"
#include <stdio.h>

namespace TaskManagement
{

TaskManager::TaskManager(fnLegacyFactory p_factory, VWMInterface *p_vwm) : TaskManagerInterface(p_vwm)
{
	s_taskMan = this;
	m_legacyFactory = p_factory;
	EnumWindows(EnumProc, reinterpret_cast<LPARAM>(this));
}

TaskManager::~TaskManager()
{
	// TODO Auto-generated destructor stub
}

void TaskManager::Reload()
{
}

void TaskManager::SwitchToWindow(HWND p_window, bool p_force)
{
	if (IsTask(p_window))
	{
		if (IsIconic(p_window)) PostMessage(p_window, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(p_window);
	}
	else
	{
		CleanTasks();
	}
}

LRESULT TaskManager::ProcessShellMessage(WPARAM p_wParam, HWND p_hWnd)
{
	switch (p_wParam)
	{
	case HSHELL_WINDOWCREATED:
		return CreateTask(p_hWnd);
	case HSHELL_WINDOWDESTROYED:
		return DestroyTask(p_hWnd);
	case HSHELL_REDRAW:
		return RedrawTask(p_hWnd, false);
	case HSHELL_FLASH:
		return RedrawTask(p_hWnd, true);
	case HSHELL_WINDOWREPLACING:
		//m_replacingWindow = p_hWnd;
		//OutputDebugStringA("We know who to replace");
		return TRUE;
	case HSHELL_WINDOWREPLACED:
		SetTimer(NULL, 0, 5000, TimerProc);
		return TRUE;
		//return ReplaceTask(p_hWnd);
	case HSHELL_WINDOWACTIVATED:
		return ActivateTask(p_hWnd, false);
	case HSHELL_RUDEAPPACTIVATED:
		return ActivateTask(p_hWnd, true);
	}
	return 0;
}

HWND TaskManager::GetTopTask()
{
	return NULL;
}

UINT TaskManager::GetTaskInfo(HWND p_window, PVOID p_info[], eTaskInfo p_infoType[], UINT p_numInfo)
{
	UINT numDone = 0;
	tTaskList::iterator taskIt= FindTask(p_window);
	if (taskIt != m_taskList.end())
	{
		Task *task = *taskIt;
		for (UINT i=0; i< p_numInfo; ++i)
		{
			switch (p_infoType[i])
			{
			case TI_LEGACY:
				p_info[i] = reinterpret_cast<PVOID>(task->getLegacy());
				numDone++;
				break;
			default:
				return numDone;
			}
		}
	}
	return numDone;
}

LRESULT TaskManager::CreateTask(HWND p_hWnd)
{
	if (IsTask(p_hWnd) && (FindTask(p_hWnd) == m_taskList.end()))
	{
		OutputDebugString(TEXT("A task!"));
		LegacyTask *newLegacy = NULL;
		if (m_legacyFactory)
		{
			newLegacy = m_legacyFactory();
		}
		Task *newTask = new Task(p_hWnd, newLegacy, m_callbacks);
		m_taskList.push_back(newTask);
		tTaskList::iterator taskIt = m_taskList.end();
		taskIt--;
		if (taskIt != m_taskList.begin())
		{
			taskIt--;
			if (newLegacy)
			{
				newLegacy->UpdatePrev(*taskIt);
			}
			LegacyTask *oldTip = (*taskIt)->getLegacy();
			if (oldTip)
			{
				oldTip->UpdateNext(newTask);
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


LRESULT TaskManager::DestroyTask(HWND p_destroyed)
{
	OutputDebugStringA("Desire destruction");
	tTaskList::iterator taskIt = FindTask(p_destroyed);
	if (taskIt != m_taskList.end())
	{
		OutputDebugStringA("Destruction");
		if (taskIt == m_taskList.begin())
		{
			tTaskList::iterator next = taskIt;
			next++;
			if (next != m_taskList.end())
			{
				(*next)->getLegacy()->UpdatePrev(NULL);
			}
			delete (*taskIt);
			m_taskList.erase(taskIt);
		}
		else
		{
			tTaskList::iterator prev = taskIt;
			tTaskList::iterator next = taskIt;
			next++;
			prev--;
			if (next != m_taskList.end())
			{
				(*next)->getLegacy()->UpdatePrev(*prev);
				(*prev)->getLegacy()->UpdateNext(*next);
			}
			else
			{
				(*prev)->getLegacy()->UpdateNext(NULL);
			}
			delete (*taskIt);
			m_taskList.erase(taskIt);
		}
		DoCallback(TASK_REMOVED, p_destroyed);
		return TRUE;

	}
	else
	{
		return FALSE;
	}
}

LRESULT TaskManager::ActivateTask(HWND p_activated, bool p_rudeApp)
{
	if (m_activeTask)
	{
		m_activeTask->Activate(false);
	}
	m_activeTask = NULL;
	tTaskList::iterator taskIt = FindTask(p_activated);
	if (taskIt != m_taskList.end())
	{
		Task *task = *taskIt;
		task->Activate(true);
		m_activeTask = task;
		DoCallback(TASK_ACTIVATED, p_activated);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LRESULT TaskManager::RedrawTask(HWND p_redraw, bool p_rudeApp)
{
	tTaskList::iterator taskIt = FindTask(p_redraw);
	if (taskIt != m_taskList.end())
	{
		Task *task = *taskIt;
		task->Update();
		task->Flash(p_rudeApp);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LRESULT TaskManager::ReplaceTask(HWND p_replaceWith)
{
	//DestroyTask(p_replaceWith);
	tTaskList::iterator taskIt = FindTask(m_replacingWindow);
	if (taskIt != m_taskList.end())
	{
		OutputDebugStringA("Replacing them");
		Task *toReplace = *taskIt;
		toReplace->ReplaceTask(p_replaceWith);
		//DestroyTask(m_replacingWindow);
		DoCallback(TASK_ADDED, p_replaceWith);
		m_replacingWindow = NULL;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

UINT TaskManager::GetNumTasks()
{
	return m_taskList.size();
}

tTaskList::iterator TaskManager::FindTask(HWND p_hWnd)
{
	for (tTaskList::iterator i = m_taskList.begin(); i != m_taskList.end(); ++i)
	{
		if ((*i)->m_hWnd == p_hWnd)
		{
			return i;
		}
	}
	return m_taskList.end();
}

WINBOOL CALLBACK TaskManager::EnumProc(HWND p_hWnd, LPARAM p_lParam)
{
	TaskManager *taskMan = reinterpret_cast<TaskManager *>(p_lParam);
	taskMan->CreateTask(p_hWnd);
	return TRUE;
}

void TaskManager::CleanTasks()
{
	for (tTaskList::iterator i = m_taskList.begin(); i != m_taskList.end(); ++i)
	{
		if (!IsTask((*i)->m_hWnd))
		{
			DestroyTask((*i)->m_hWnd);
			CleanTasks();
			break;
		}
	}
}

VOID CALLBACK TaskManager::TimerProc(HWND p_hWnd, UINT p_uMsg, UINT_PTR p_idEvent, DWORD p_dwTime)
{
	KillTimer(NULL, p_idEvent);
	s_taskMan->CleanTasks();
}

TaskManager *TaskManager::s_taskMan;

}
