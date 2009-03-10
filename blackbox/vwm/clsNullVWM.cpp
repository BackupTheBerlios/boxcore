/*
 * clsNullVWM.cpp
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#include "clsNullVWM.h"

namespace TaskManagement
{

NullVWM::NullVWM()
{
	m_primaryScreenSizeX = GetSystemMetrics(SM_CXSCREEN);
	m_primaryScreenSizeY = GetSystemMetrics(SM_CYSCREEN);

	m_virtualScreenSizeX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_virtualScreenSizeY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	m_virtualScreenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	m_virtualScreenY = GetSystemMetrics(SM_YVIRTUALSCREEN);
}

NullVWM::~NullVWM()
{
	// TODO Auto-generated destructor stub
}

void NullVWM::Reload()
{
}

void NullVWM::Update()
{
}

void NullVWM::SwitchToWorkspace(HMONITOR p_monitor, UINT p_workspace)
{
}

void NullVWM::GatherWindows()
{
}

UINT NullVWM::GetWindowWorkspace(HWND p_window)
{
	return 0;
}

UINT NullVWM::GetWindowStatus(HWND p_window)
{
	return 0;
}

void NullVWM::SetWindowWorkspace(HWND p_hwnd, HMONITOR p_monitor, UINT p_workspace, bool p_switch)
{

}

UINT NullVWM::GetCurrentWorkspace(HMONITOR p_monitor)
{
	return 0;
}

LPCTSTR NullVWM::GetWorkspaceName(HMONITOR p_monitor, UINT p_workspace)
{
	return TEXT("Desktop");
}

UINT NullVWM::GetNumWorkspaces(HMONITOR p_monitor)
{
	return 1;
}

void NullVWM::PrevWorkspace(HMONITOR p_monitor) {}
void NullVWM::NextWorkspace(HMONITOR p_monitor) {}

int NullVWM::GetVirtualScreenX()
{
	return m_virtualScreenX;
}

int NullVWM::GetVirtualScreenY()
{
	return m_virtualScreenY;
}

int NullVWM::GetVirtualScreenSizeX()
{
	return m_virtualScreenSizeX;
}

int NullVWM::GetVirtualScreenSizeY()
{
	return m_virtualScreenSizeY;
}

int NullVWM::GetPrimaryScreenSizeX()
{
	return m_primaryScreenSizeX;
}

int NullVWM::GetPrimaryScreenSizeY()
{
	return m_primaryScreenSizeY;
}

}
