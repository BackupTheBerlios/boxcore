#include "clsTrayItem.h"
#include <list>

using std::list;



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
