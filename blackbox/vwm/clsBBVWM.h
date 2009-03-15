/*
 * BBVWM.h
 *
 *  Created on: 15 Mar 2009
 *      Author: shaun
 */

#ifndef CLSBBVWM_H_
#define CLSBBVWM_H_

#include "clsVWMInterface.h"

namespace TaskManagement
{

class BBVWM: public VWMInterface
{
public:
	BBVWM();
	virtual ~BBVWM();

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

	UINT m_currentWorkspace;
	UINT m_numWorkspaces;
};

}

#endif /* CLSBBVWM_H_ */
