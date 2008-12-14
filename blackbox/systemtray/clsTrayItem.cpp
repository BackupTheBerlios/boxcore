/** @internal
  * @file clsTrayItem.cpp
  * @brief This file contains the implementation of clsTrayItem for the system tray
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

#include "clsTrayItem.h"

clsTrayItem::clsTrayItem(NID_INTERNAL &pNID)
{
	valid = true;
	showTooltip = true;
	if (pNID.uFlags&(NIF_MESSAGE|NIF_ICON))
	{
		hWnd = pNID.hWnd;
		iconID = pNID.uID;
		callbackMessage = pNID.uCallbackMessage;
		stateShared = false;
		stateHidden = false;
	}
	else
	{
		///When insuffiecient information is present to create a tray icon, we set the TrayItem as invalid
		///and return immediately. The minimum requirements we enforce are an icon and a callback message,
		///so something can be displayed and a window can be notified of actions on the tray icon.
		valid = false;
		return;
	}
}

clsTrayItem::~clsTrayItem()
{
	//dtor
}
