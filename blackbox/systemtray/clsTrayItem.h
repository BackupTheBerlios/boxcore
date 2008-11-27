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

/**
 * @class clsTrayItem
 *
 * @brief Contains data for a single tray item
 */
class clsTrayItem
{
	public:
		clsTrayItem(NID_INTERNAL &pNID);
		virtual ~clsTrayItem();

		bool constructionValid() {return valid;}
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
