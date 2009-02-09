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
	virtual UINT GetWindowDesk(HWND p_window);
	virtual UINT GetWindowStatus(HWND p_window);
};

}

#endif /* CLSNULLVWM_H_ */
