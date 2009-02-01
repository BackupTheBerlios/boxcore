/** @internal
  * @file managers.h
  * @brief Header to include for interactions with shell managers
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2008 Carsomyr -->
  * Copyright &copy; 2008 Carsomyr
  * @par links
  * http://developer.berlios.de/projects/boxcore @n
  * @par License
  * boxCore is free software, released under the GNU General
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

#ifndef MANAGERS_H
#define MANAGERS_H

#include "shellserviceobjects/clsShellServiceObjects.h"
#include "shellservices/clsNotifyIconHandler.h"
#include "clsSystemInfo.h"
#include <map>

extern clsShellServiceObjects ShellServiceObjectsManager;
extern clsSystemInfo SystemInfo;
extern ShellServices::NotifyIconHandler *g_pNotificationIconHandler;
extern std::map<ATOM,ShellServices::eNotificationIconInfo> g_trayInfoMapping;

#include "clsMessageManager.h"
extern MessageManager *g_pMessageManager;

#endif
