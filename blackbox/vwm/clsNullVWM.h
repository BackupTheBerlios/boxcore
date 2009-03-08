/*
 * clsNullVWM.h
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#ifndef CLSNULLVWM_H_
#define CLSNULLVWM_H_

#include "clsVWMInterface.h"

namespace TaskManagement
{

class NullVWM: public VWMInterface
{
public:
	NullVWM();
	virtual ~NullVWM();

	virtual void Reload();
	virtual void Update();

	virtual void SwitchToWorkspace(HMONITOR p_monitor, UINT p_workspace);
	virtual void GatherWindows();
	virtual void SetWindowWorkspace(HWND p_hwnd, HMONITOR p_monitor, UINT p_workspace, bool p_switch);
	virtual UINT GetWindowWorkspace(HWND p_window);
	virtual UINT GetWindowStatus(HWND p_window);
	virtual UINT GetCurrentWorkspace(HMONITOR p_monitor);
	virtual LPCTSTR GetWorkspaceName(HMONITOR p_monitor, UINT p_workspace);
	virtual UINT GetNumWorkspaces(HMONITOR p_monitor);

	virtual void PrevWorkspace(HMONITOR _monitor);
	virtual void NextWorkspace(HMONITOR _monitor);
};

}

#endif /* CLSNULLVWM_H_ */
