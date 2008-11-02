#ifndef CLSTRAYITEM_H
#define CLSTRAYITEM_H
#include "windows.h"
#include <string>

using std::wstring;

/**
 * @class clsTrayItem
 *
 * @brief Contains data for a single tray item
 */
class clsTrayItem
{
	public:
		clsTrayItem();
		virtual ~clsTrayItem();
	protected:
	private:
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
		///Tooltip for this icon
		wstring tooltip;
		///Should the icon be hidden
		bool stateHidden;
		///Is the icon shared
		bool stateShared;
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
};

#endif // CLSTRAYITEM_H
