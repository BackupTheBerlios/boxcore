/*
 * clsTask.cpp
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#include "clsTask.h"
#include "clsLegacyTask.h"
#include "helpers.h"

namespace TaskManagement
{

Task::Task(HWND p_hWnd, LegacyTask *p_legacy, tTaskCallbackMap &p_callbacks) : m_callbacks(p_callbacks)
{
	m_smallIcon = NULL;
	m_largeIcon = NULL;
	m_origSmallIcon = NULL;
	m_origLargeIcon = NULL;
	m_caption[0] = L'\0';
	m_hWnd = p_hWnd;
	m_active = false;
	m_flashing = false;
	m_legacy = p_legacy;
	m_newTask = true;
	Update();
}

Task::~Task()
{
	if (m_smallIcon)
	{
		DestroyIcon(m_smallIcon);
	}
	if (m_largeIcon)
	{
		DestroyIcon(m_largeIcon);
	}
}

void Task::ReplaceTask(HWND p_hWnd)
{
	m_hWnd = p_hWnd;
	if (m_smallIcon)
	{
		DestroyIcon(m_smallIcon);
	}
	if (m_largeIcon)
	{
		DestroyIcon(m_largeIcon);
	}
	m_origSmallIcon = NULL;
	m_origLargeIcon = NULL;
	Update();
}

void Task::Flash(bool p_status)
{
	m_flashing = p_status;
	UpdateLegacy();

	if (p_status)
	{
		DoCallback(TASK_FLASHED, m_hWnd, NULL);
	}
}

void Task::Activate(bool p_status)
{
	m_active = p_status;
	UpdateLegacy();
}

void Task::Update()
{
	GetWindowTextW(m_hWnd, m_caption, sizeof(m_caption)/sizeof(WCHAR));
	SendMessageCallback(m_hWnd, WM_GETICON, ICON_SMALL, 0, SmallIconProc, reinterpret_cast<ULONG_PTR>(this));
}

void Task::UpdateLegacy()
{
	if (m_legacy)
	{
		m_legacy->Update(this);
	}
}

VOID CALLBACK Task::SmallIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult)
{
	OutputDebugString("Trying SMALL");
	if (IsTask(p_hWnd))
	{
		Task *task = reinterpret_cast<Task *>(p_dwData);;
		if (p_lResult)
		{
			task->SetSmallIcon(reinterpret_cast<HICON>(p_lResult));
		}
		else
		{
			task->SetSmallIcon(reinterpret_cast<HICON>(GetClassLongPtr(task->m_hWnd, GCLP_HICONSM)));
		}
		SendMessageCallback(task->m_hWnd, WM_GETICON, ICON_BIG, 0, LargeIconProc, reinterpret_cast<ULONG_PTR>(task));
	}
}

VOID CALLBACK Task::LargeIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult)
{
	OutputDebugString("Trying LARGE");
	if (IsTask(p_hWnd))
	{
		Task *task = reinterpret_cast<Task *>(p_dwData);
		if (p_lResult)
		{
			task->SetLargeIcon(reinterpret_cast<HICON>(p_lResult));
		}
		else
		{
			task->SetLargeIcon(reinterpret_cast<HICON>(GetClassLongPtr(task->m_hWnd, GCLP_HICON)));
			if (!task->m_largeIcon && task->m_smallIcon)
			{
				task->SetLargeIcon(task->m_origSmallIcon);
			}
		}

		if (!task->m_smallIcon)
		{
			task->SetSmallIcon(task->m_origLargeIcon);
		}

		task->UpdateLegacy();

		if (task->m_newTask)
		{
			task->m_newTask = false;
			task->DoCallback(TASK_ADDED, task->m_hWnd, NULL);
		}
		else
		{
			task->DoCallback(TASK_UPDATED, task->m_hWnd, NULL);
		}
	}
}

void Task::SetSmallIcon(HICON p_icon)
{
	if (p_icon && p_icon != m_origSmallIcon)
	{
		if (m_smallIcon)
		{
			DestroyIcon(m_smallIcon);
			m_smallIcon = NULL;
		}
		m_origSmallIcon = p_icon;
		m_smallIcon = CopyIcon(m_origSmallIcon);
	}
}

void Task::SetLargeIcon(HICON p_icon)
{
	if (p_icon && p_icon != m_origLargeIcon)
	{
		if (m_largeIcon)
		{
			DestroyIcon(m_largeIcon);
			m_largeIcon = NULL;
		}
		m_origLargeIcon = p_icon;
		m_largeIcon = CopyIcon(m_origLargeIcon);
	}
}

void Task::DoCallback(eTaskCallbackType p_type, HWND p_window, PVOID p_extra)
{
	tTaskCallbackMap::iterator callbackIt = m_callbacks.find(p_type);
	if (callbackIt != m_callbacks.end())
	{
		callbackIt->second(p_window, p_extra);
	}
}

}
