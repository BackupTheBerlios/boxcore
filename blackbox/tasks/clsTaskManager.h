/*
 * clsTaskManager.h
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#ifndef CLSTASKMANAGER_H_
#define CLSTASKMANAGER_H_

#include "clsTaskManagerInterface.h"
#include <list>

namespace TaskManagement
{

class Task;

typedef std::list<Task *> tTaskList;

class TaskManager: public TaskManagerInterface
{
public:
	TaskManager(VWMInterface *p_vwm);
	virtual ~TaskManager();

	virtual void Reload();
	virtual void SwitchToWindow(HWND p_window, bool p_force);
	virtual LRESULT ProcessShellMessage(WPARAM p_wParam, HWND p_hWnd);
	virtual HWND GetTopTask();
	virtual UINT GetTaskInfo(HWND p_window, PVOID p_info[], ATOM p_infoType[], UINT p_numInfo);
private:
	tTaskList m_taskList;
	HWND m_replacingWindow;

	bool IsTask(HWND p_hWnd);

	LRESULT CreateTask(HWND p_hWnd);
	LRESULT DestroyTask(HWND p_hWnd);
	LRESULT ActivateTask(HWND p_hWnd, bool p_rudeApp);
	LRESULT RedrawTask(HWND p_hWnd, bool p_rudeApp);
	LRESULT ReplaceTask(HWND m_replace, HWND p_replaceWith);
};

}

#endif /* CLSTASKMANAGER_H_ */
