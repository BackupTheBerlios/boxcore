#ifndef CLSTRAYITEM_H
#define CLSTRAYITEM_H

#include "BBApi.h"
#include "clsIconItem.h"

typedef BOOL(*fnGetTrayInfo)(HWND, UINT, PVOID*, ATOM*, UINT);

/** @class clsTrayItem
  * @brief Represents a single tray item in the bar
  */
class clsTrayItem : public clsIconItem
{
public:
	clsTrayItem(systemTray *trayItem, UINT pIconSize, bool pVertical);

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	HWND iconWnd;
	UINT iconID;
	UINT iconCallback;
	UINT m_version;
	bool popupVisible;

	static fnGetTrayInfo GetTrayInfo;
private:
};

#endif // CLSTRAYITEM_H
