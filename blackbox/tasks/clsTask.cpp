/*
 * clsTask.cpp
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#include "clsTask.h"

namespace TaskManagement
{

Task::Task(HWND p_hWnd)
{
	m_smallIcon = NULL;
	m_largeIcon = NULL;
	m_caption[0] = L'\0';
	m_hWnd = p_hWnd;
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
	Update();
}

void Task::Flash(bool p_status)
{
	m_flashing = p_status;
}

void Task::Activate(bool p_status)
{
	m_active = p_status;
}

void Task::Update()
{
	GetWindowTextW(m_hWnd, m_caption, sizeof(m_caption)/sizeof(WCHAR));
	HICON icon = reinterpret_cast<HICON>(GetClassLongPtr(m_hWnd, GCLP_HICONSM));
	if (icon && icon != m_origSmallIcon)
	{
		if (m_smallIcon)
		{
			DestroyIcon(m_smallIcon);
		}
		m_origSmallIcon = icon;
		m_smallIcon = CopyIcon(m_origSmallIcon);
	}
	icon = reinterpret_cast<HICON>(GetClassLongPtr(m_hWnd, GCLP_HICON));
	if (icon && icon != m_origLargeIcon)
	{
		if (m_largeIcon)
		{
			DestroyIcon(m_largeIcon);
		}
		m_origLargeIcon = icon;
		m_largeIcon = CopyIcon(m_origLargeIcon);
	}
}

}
