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
class LegacyTask;

typedef LegacyTask *(*fnLegacyFactory)();

typedef std::list<Task *> tTaskList;

class TaskManager: public TaskManagerInterface
{
public:
	TaskManager(fnLegacyFactory p_factory, VWMInterface *p_vwm);
	virtual ~TaskManager();

	virtual void Reload();
	virtual void SwitchToWindow(HWND p_window, bool p_force);
	virtual LRESULT ProcessShellMessage(WPARAM p_wParam, HWND p_hWnd);
	virtual HWND GetTopTask();
	virtual UINT GetTaskInfo(HWND p_window, PVOID p_info[], eTaskInfo p_infoType[], UINT p_numInfo);

	virtual void CleanTasks();
	virtual UINT GetNumTasks();
	virtual HWND GetTaskWindow(UINT p_taskNum);
private:
	fnLegacyFactory m_legacyFactory;
	tTaskList m_taskList;
	HWND m_replacingWindow;
	Task *m_activeTask;

	static BOOL CALLBACK EnumProc(HWND p_hWnd, LPARAM p_lParam);

	static VOID CALLBACK TimerProc(HWND p_hWnd, UINT p_uMsg, UINT_PTR p_idEvent, DWORD p_dwTime);
	static TaskManager *s_taskMan;

	LRESULT CreateTask(HWND p_created);
	LRESULT DestroyTask(HWND p_destroyed);
	LRESULT ActivateTask(HWND p_activated, bool p_rudeApp);
	LRESULT RedrawTask(HWND p_redraw, bool p_rudeApp);
	LRESULT ReplaceTask(HWND p_hWnd);
	tTaskList::iterator FindTask(HWND p_hWnd);
};

}

#endif /* CLSTASKMANAGER_H_ */
