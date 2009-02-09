/** @internal
  * @brief Defines the interface that Task Management modules must implement
  *
  * This interface has a constructor which forces modules to initialise the VWM reference
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright (C) 2009 Carsomyr -->
  * Copyright &copy; 2009 Carsomyr
  * @par Links:
  * http://developer.berlios.de/projects/boxcore @n
  * @par License:
  * boxCore, bbLean and bb4win are free software, released under the GNU General
  * Public License (GPL version 2 or later), with an extension that allows
  * linking of proprietary modules under a controlled interface. This means
  * that plugins etc. are allowed to be released under any license the author
  * wishes. For details see:
  * @par
  * http://www.fsf.org/licenses/gpl.html @n
  * http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface
  * @par
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  */

#ifndef CLSTASKMANAGERINTERFACE_H_
#define CLSTASKMANAGERINTERFACE_H_

#define _WIN32_WINNT 0x0501

#include "../vwm/clsVWMInterface.h"
#include <map>

namespace TaskManagement
{

typedef void (*fnTaskCallback)(HWND p_window);
enum eTaskCallbackType {TASK_ADDED};

class TaskManagerInterface
{
public:
	TaskManagerInterface(VWMInterface *p_vwm);
	virtual ~TaskManagerInterface();

	virtual void Reload() PURE;
	virtual void SwitchToWindow(HWND p_window, bool p_force) PURE;
	virtual LRESULT ProcessShellMessage(WPARAM p_wParam, HWND p_hWnd) PURE;

	virtual HWND GetTopTask() PURE;

	virtual UINT GetTaskInfo(HWND p_window, PVOID p_info[], ATOM p_infoType[], UINT p_numInfo) PURE;

	VWMInterface *GetVWM();
	void RegisterCallback(eTaskCallbackType p_type, fnTaskCallback p_callback);
protected:
	std::map<eTaskCallbackType, fnTaskCallback> m_callbacks;
	VWMInterface *m_vwm;
};

}

#endif /* CLSTASKMANAGERINTERFACE_H_ */
