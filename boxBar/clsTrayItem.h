#ifndef CLSTRAYITEM_H
#define CLSTRAYITEM_H

#include "BBApi.h"
#include "clsIconItem.h"

typedef BOOL(*fnTrayIconEvent)(HWND, UINT, UINT, WPARAM, LPARAM);

class clsTrayItem : public clsIconItem
{
	public:
		clsTrayItem(systemTray *trayItem, bool pVertical);
		virtual ~clsTrayItem();

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	protected:
		HWND iconWnd;
		UINT iconID;
		UINT iconCallback;
		bool popupVisible;

		static fnTrayIconEvent TrayIconEvent;
	private:
};

#endif // CLSTRAYITEM_H
