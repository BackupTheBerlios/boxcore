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
	// TODO Auto-generated constructor stub

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

void NullVWM::PrevWorkspace(HMONITOR _monitor) {}
void NullVWM::NextWorkspace(HMONITOR _monitor) {}

}
