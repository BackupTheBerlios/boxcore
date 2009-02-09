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

UINT NullVWM::GetWindowDesk(HWND p_window)
{
	return 1;
}

UINT NullVWM::GetWindowStatus(HWND p_window)
{
	return 0;
}

}
