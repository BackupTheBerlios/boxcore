/*
 * BBVWM.cpp
 *
 *  Created on: 15 Mar 2009
 *      Author: shaun
 */

#include "clsBBVWM.h"

namespace TaskManagement
{

BBVWM::BBVWM()
{
	// TODO Auto-generated constructor stub
	m_primaryScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	m_primaryScreenSizeY = GetSystemMetrics(SM_CYSCREEN);

	m_virtualScreenSizeX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_virtualScreenSizeY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	m_virtualScreenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	m_virtualScreenY = GetSystemMetrics(SM_YVIRTUALSCREEN);

	m_numWorkspaces = 4;
	m_currentWorkspace = 1;
}

BBVWM::~BBVWM()
{
	// TODO Auto-generated destructor stub
}

void BBVWM::Reload()
{
	m_primaryScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	m_primaryScreenSizeY = GetSystemMetrics(SM_CYSCREEN);

	m_virtualScreenSizeX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_virtualScreenSizeY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	m_virtualScreenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	m_virtualScreenY = GetSystemMetrics(SM_YVIRTUALSCREEN);
}

void BBVWM::Update()
{
}

void BBVWM::SwitchToWorkspace(HMONITOR p_monitor, UINT p_workspace)
{
}

void BBVWM::GatherWindows()
{
}

void BBVWM::SetWindowWorkspace(HWND p_hwnd, HMONITOR p_monitor, UINT p_workspace, bool p_switch)
{
}

UINT BBVWM::GetWindowWorkspace(HWND p_window)
{
	return 0;
}

UINT BBVWM::GetWindowStatus(HWND p_window)
{
	return 0;
}

UINT BBVWM::GetCurrentWorkspace(HMONITOR p_monitor)
{
	return m_currentWorkspace - 1;
}

LPCTSTR BBVWM::GetWorkspaceName(HMONITOR p_monitor, UINT p_workspace)
{
	switch (p_workspace)
	{
	default:
	case 0:
		return "One";
	case 1:
		return "Two";
	case 2:
		return "Three";
	case 3:
		return "Four";
	}
}

UINT BBVWM::GetNumWorkspaces(HMONITOR p_monitor)
{
	return m_numWorkspaces;
}

int BBVWM::GetVirtualScreenX()
{
	return m_virtualScreenX;
}

int BBVWM::GetVirtualScreenY()
{
	return m_virtualScreenY;
}

int BBVWM::GetVirtualScreenSizeX()
{
	return m_virtualScreenSizeX;
}

int BBVWM::GetVirtualScreenSizeY()
{
	return m_virtualScreenSizeY;
}

int BBVWM::GetPrimaryScreenSizeX()
{
	return m_primaryScreenSizeX;
}

int BBVWM::GetPrimaryScreenSizeY()
{
	return m_primaryScreenSizeY;
}

void BBVWM::PrevWorkspace(HMONITOR p_monitor)
{
	--m_currentWorkspace;
	if (m_currentWorkspace == 0)
		m_currentWorkspace = m_numWorkspaces;
}

void BBVWM::NextWorkspace(HMONITOR p_monitor)
{
	++m_currentWorkspace;
	if (m_currentWorkspace > m_numWorkspaces)
		m_currentWorkspace = 1;
}

}
