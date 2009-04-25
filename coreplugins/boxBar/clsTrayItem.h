#ifndef CLSTRAYITEM_H
#define CLSTRAYITEM_H

#include "BBApi.h"
#include "clsIcon.h"

namespace boxBar
{

typedef BOOL(*fnGetTrayInfo)(HWND, UINT, PVOID*, ATOM*, UINT);

/** @internal
 * @class TrayIcon
 * @brief Represents a single tray item in the bar
 */
class TrayIcon : public Icon
{
public:
	TrayIcon(systemTray *trayItem, UINT pIconSize);

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

}

#endif // CLSTRAYITEM_H
