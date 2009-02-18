/** @internal
  * @brief Contains the constructor for the TaskManagerInterface
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

#include "clsTaskManagerInterface.h"

namespace TaskManagement
{

TaskManagerInterface::TaskManagerInterface(VWMInterface *p_vwm)
{
	InitializeCriticalSection(&m_critSection);
	m_vwm = p_vwm;
}

TaskManagerInterface::~TaskManagerInterface()
{
	DeleteCriticalSection(&m_critSection);
}

VWMInterface *TaskManagerInterface::GetVWM()
{
	return m_vwm;
}

void TaskManagerInterface::RegisterCallback(eTaskCallbackType p_type, fnTaskCallback p_callback)
{
	if (p_callback)
	{
		m_callbacks[p_type] = p_callback;
	}
	else
	{
		m_callbacks.erase(p_type);
	}
}

void TaskManagerInterface::DoCallback(eTaskCallbackType p_type, HWND p_window)
{
	tTaskCallbackMap::iterator callbackIt = m_callbacks.find(p_type);
	if (callbackIt != m_callbacks.end())
	{
		callbackIt->second(p_window);
	}
	/// @todo Update the VWM as well here
}

}
