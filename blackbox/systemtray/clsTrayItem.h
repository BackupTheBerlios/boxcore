/** @internal
  * @file clsTrayItem.h
  * @brief This file contains the definition of clsTrayItem for use by the system tray
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

#ifndef CLSTRAYITEM_H
#define CLSTRAYITEM_H
#include "windows.h"
#include <string>

using std::wstring;

struct NID_INTERNAL
{
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union{
		UINT uTimeout;
		UINT uVersion;
	};
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
	HICON hBalloonItem;
};

/** @internal
  * @class clsTrayItem
  * @brief Contains data for a single tray item
  */
class clsTrayItem
{
public:
	clsTrayItem(NID_INTERNAL &pNID);
	virtual ~clsTrayItem();

	bool constructionValid() {
		return valid;
	}
	//protected:
	//private:
	///Window which will recieve notifications from this icon
	HWND hWnd;
	///Identifier for this icon
	UINT iconID;
	///Whether to show the standard tooltip or not. (Always true, unless under Vista)
	bool showTooltip;
	///Contains the message use to communicate with the owning window
	UINT callbackMessage;
	///The icon to display in the tray
	HICON hIcon;
	///The original icon handle (Used when icons is shared)
	HICON hIconOrig;
	///Tooltip for this icon
	wstring tooltip;
	///Should the icon be hidden
	bool stateHidden;
	///Is the icon shared
	bool stateShared;
	///number of references if shared
	int iconRefcount;
	///Text for a balloon tooltip
	wstring balloonTip;
	///Timout value for the balloon tooltip @todo Limit to between 10 and 30 seconds
	UINT balloonTimeout;
	///Specisfies the desired version of tray behaviour @todo Very important for vista
	UINT version;
	/// Icon for the balloon tooltip, if large icons are enabled @since Windows XP
	HICON hBalloonIcon;
	/// Reserved @since Windows Vista
	GUID itemGuid;

	bool valid;

	friend class clsSystemTray;
};

#endif // CLSTRAYITEM_H
