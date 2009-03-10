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

	virtual int GetVirtualScreenX();
	virtual int GetVirtualScreenY();
	virtual int GetVirtualScreenSizeX();
	virtual int GetVirtualScreenSizeY();
	virtual int GetPrimaryScreenSizeX();
	virtual int GetPrimaryScreenSizeY();

	virtual void PrevWorkspace(HMONITOR p_monitor);
	virtual void NextWorkspace(HMONITOR p_monitor);
private:
	int m_virtualScreenX;
	int m_virtualScreenY;
	int m_virtualScreenSizeX;
	int m_virtualScreenSizeY;
	int m_primaryScreenSizeX;
	int m_primaryScreenSizeY;
};

}

#endif /* CLSNULLVWM_H_ */
