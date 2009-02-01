/** @internal
  * @file
  * @brief This file contains the implementation of the MessageManager class.
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

#include "BBApi.h"
#include "clsMessageManager.h"

/** @brief Sends the specified message to all subscribers
 *
 * @param msg[in] The message to broadcast
 * @param wParam[in] The wParam of the message to broadcast
 * @param lParam[in] The lParam of the message to broadcast
 * @return The returned value from the last message
 *
 * This function returns after the first non-zero return value is received for BB_DRAGTODESKTOP
 * to match the original bbLean implementation
 */
LRESULT MessageManager::BroadcastMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	tMessageMap::iterator mapEntry = m_messageMap.find(msg);
	if (mapEntry != m_messageMap.end())
	{
	tWindowList &windowList = mapEntry->second;
	LRESULT retVal = 0;
	for(tWindowList::iterator i = windowList.begin(); i != windowList.end(); ++i)
	{
		retVal = SendMessage(*i, msg, wParam, lParam);
		if (retVal && msg == BB_DRAGTODESKTOP)
		{
			return retVal;
		}
	}
	return retVal;
	}
	else
	{
		return 0;
	}
}

/** @brief Registers a list of message a window wishes to receive
  *
  * @param p_window The window for which message are being registered
  * @param p_messages An array of message for which the window is registering, the last element must be 0
  */
void MessageManager::AddMessages(HWND p_window, UINT *p_messages)
{
	while(*p_messages != 0)
	{
		m_messageMap[*p_messages].insert(p_window);
		++p_messages;
	}
}

/** @brief Deregisters a list of message a window no longer wishes to receive
  *
  * @param p_window The window for which message are being registered
  * @param p_messages An array of message for which the window is deregistering, the last element must be 0
  */
void MessageManager::RemoveMessages(HWND p_window, UINT *p_messages)
{
	while(*p_messages != 0)
	{
		m_messageMap[*p_messages].erase(p_window);
		if (m_messageMap[*p_messages].size() == 0)
		{
			m_messageMap.erase(*p_messages);
		}
		++p_messages;
	}
}






