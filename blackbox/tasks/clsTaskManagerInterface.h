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

#include "../vwm/clsVWMInterface.h"

namespace WindowManagement
{

class TaskManagerInterface
{
public:
	TaskManagerInterface(VWMInterface p_vwm);
	virtual ~TaskManagerInterface();
};

}

#endif /* CLSTASKMANAGERINTERFACE_H_ */
