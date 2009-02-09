/*
 * clsTaskManager.cpp
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#include "clsTaskManager.h"
#include "clsTask.h"

namespace TaskManagement
{

TaskManager::TaskManager(VWMInterface *p_vwm) : TaskManagerInterface(p_vwm)
{
	// TODO Auto-generated constructor stub

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
		m_replacingWindow = p_hWnd;
		return TRUE;
	case HSHELL_WINDOWREPLACED:
		return ReplaceTask(m_replacingWindow, p_hWnd);
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

UINT TaskManager::GetTaskInfo(HWND p_window, PVOID p_info[], ATOM p_infoType[], UINT p_numInfo)
{
	UINT numDone = 0;
	return numDone;
}

bool TaskManager::IsTask(HWND p_hWnd)
{
	if (!IsWindow(p_hWnd))
	{
		return false;
	}

	if (GetParent(p_hWnd) || !IsWindowVisible(p_hWnd))
	{
		return false;
	}

	LONG_PTR exStyle = GetWindowLongPtr(p_hWnd, GWL_EXSTYLE);
	HWND owner = GetWindow(p_hWnd, GW_OWNER);
	if (exStyle & WS_EX_TOOLWINDOW)
	{
		return true;
	}
	if ((exStyle & WS_EX_TOOLWINDOW) && owner)
	{
		return false;
	}

	return true;
}

LRESULT TaskManager::CreateTask(HWND p_hWnd)
{
	if (IsTask(p_hWnd))
	{
		m_taskList.push_back(new Task(p_hWnd));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


LRESULT TaskManager::ReplaceTask(HWND m_replace, HWND p_replaceWith)
{
	return ProcessShellMessage(HSHELL_REDRAW, p_replaceWith);
}

}
