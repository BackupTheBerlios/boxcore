/** @internal
  * @file
  * @brief This file contains the definition of the MessageManager class.
  *
  * This file is part of the boxCore source code @n
  * <!-- Copyright © 2009 Carsomyr -->
  * Copyright &copy; 2009 Carsomyr
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

#ifndef CLSMESSAGEMANAGER_H_
#define CLSMESSAGEMANAGER_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <set>
#include <map>

typedef std::set<HWND> tWindowList; ///< Type that keeps track of lists of windows subscribed to a given message
typedef std::map<UINT, tWindowList> tMessageMap; ///< Type links messages with ::tWindowList

/** @internal
  * @brief This class handles registration for and broadcasting of internal blackbox messages
  */
class MessageManager
{
public:
	void AddMessages(HWND p_window, UINT* p_messages);
	void RemoveMessages(HWND p_window, UINT* p_messages);
	LRESULT BroadcastMessage(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	tMessageMap m_messageMap; ///< @brief Maps messages to lists of windows which registered for them
};

#endif /* CLSMESSAGEMANAGER_H_ */
