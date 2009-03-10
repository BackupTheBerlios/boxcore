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

#include "../../debug/debug.h"

namespace TaskManagement
{

/** @internal
  * @brief Structure for the LPARAM of HSHELL_GETMINRECT messages
  *
  * This structure uses SHORT's for the coordinates rather than the RECT that is documented.
  * The SHORT's work, while if you make it a RECT it crashes or just does nothing.
  * Credit to jimrandomh from #lsdev for figuring this out.
  */
struct MINRECTINFO
{
	HWND hWnd;
	SHORT left;
	SHORT top;
	SHORT right;
	SHORT bottom;
};

TaskManager::TaskManager(fnLegacyFactory p_factory, VWMInterface *p_vwm) : TaskManagerInterface(p_vwm)
{
	s_taskMan = this;
	m_replacingWindow = NULL;
	m_legacyFactory = p_factory;
	m_activeTask = NULL;
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
		break;
	case HSHELL_WINDOWDESTROYED:
		return DestroyTask(p_hWnd);
	case HSHELL_REDRAW:
		return RedrawTask(p_hWnd, false);
	case HSHELL_FLASH:
		return RedrawTask(p_hWnd, true);
	case HSHELL_WINDOWREPLACING:
		m_replacingWindow = p_hWnd;
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
	case HSHELL_GETMINRECT:
	{
		MINRECTINFO &info = *reinterpret_cast<MINRECTINFO *>(p_hWnd);
		TRACE("The window is %p", info.hWnd);
		DoCallback(TASK_GETRECT, info.hWnd, &info.left);
		return TRUE;
	}
	}
	return 0;
}

HWND TaskManager::GetTopTask()
{
	return m_activeTask->getHWnd();
}

void TaskManager::FocusTopTask(HWND p_exclude)
{
	EnumWindows(FocusTopTaskEnumExternal, reinterpret_cast<LPARAM>(this));
}

BOOL CALLBACK TaskManager::FocusTopTaskEnumExternal(HWND p_hWnd, LPARAM p_lParam)
{
	TaskManager *caller = reinterpret_cast<TaskManager *>(p_lParam);
	return caller->FocusTopTaskEnum(p_hWnd, p_lParam);
}

BOOL CALLBACK TaskManager::FocusTopTaskEnum(HWND p_hWnd, LPARAM p_lParam)
{
	if (IsTask(p_hWnd))
	{
		tTaskList::iterator task = FindTask(p_hWnd);
		if (m_vwm->GetWindowWorkspace(p_hWnd) == m_vwm->GetCurrentWorkspace(MonitorFromWindow(p_hWnd, MONITOR_DEFAULTTOPRIMARY)))
		{
			ActivateTask(p_hWnd, false);
			return FALSE;
		}
	}
	return TRUE;
}

HWND TaskManager::GetTaskWindow(UINT p_taskNum)
{
	if (p_taskNum<m_taskList.size())
	{
		tTaskList::iterator task = m_taskList.begin();
		advance(task,p_taskNum);
		return (*task)->getHWnd();
	}
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
			case TI_ACTIVE:
				p_info[i] = reinterpret_cast<PVOID>(task->getActive());
				numDone++;
				break;
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
		if (m_vwm)
		{
			HMONITOR mon = MonitorFromWindow(p_hWnd, MONITOR_DEFAULTTOPRIMARY);
			m_vwm->SetWindowWorkspace(p_hWnd, mon, m_vwm->GetCurrentWorkspace(mon), false);
		}
		tTaskList::iterator taskIt = m_taskList.insert(m_taskList.end(), newTask);
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
	tTaskList::iterator taskIt = FindTask(p_destroyed);
	if (taskIt != m_taskList.end())
	{
		if (taskIt == m_taskList.begin())
		{
			tTaskList::iterator next = taskIt;
			next++;
			if (next != m_taskList.end())
			{
				(*next)->getLegacy()->UpdatePrev(NULL);
			}
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
		}
		if (m_activeTask == *taskIt)
			{
				m_activeTask = NULL;
			}
			delete (*taskIt);
			m_taskList.erase(taskIt);
		DoCallback(TASK_REMOVED, p_destroyed, NULL);
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
	tTaskList::iterator taskIt = FindTask(p_activated);
	if (taskIt != m_taskList.end())
	{
		Task *task = *taskIt;
		task->Activate(true);
		m_activeTask = task;
		DoCallback(TASK_ACTIVATED, p_activated, NULL);
		return TRUE;
	}
	else
	{
		m_activeTask = NULL;
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
		DoCallback(TASK_ADDED, p_replaceWith, NULL);
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

BOOL CALLBACK TaskManager::EnumProc(HWND p_hWnd, LPARAM p_lParam)
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
