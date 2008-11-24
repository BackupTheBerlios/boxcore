#ifndef CLSTRAYITEM_H
#define CLSTRAYITEM_H

#include "BBApi.h"
#include "clsItem.h"

typedef BOOL(*fnTrayIconEvent)(HWND, UINT, UINT, WPARAM, LPARAM);

class clsTrayItem : public clsItem
{
	public:
		clsTrayItem(systemTray *trayItem, bool pVertical);
		virtual ~clsTrayItem();

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void draw(HDC pContext);
		virtual void calculateSizes(bool pSizeGiven = false);
	protected:
		HWND iconWnd;
		HICON iconIcon;
		UINT iconID;
		UINT iconCallback;
		bool popupVisible;

		int iconSize;

		static fnTrayIconEvent TrayIconEvent;
	private:
};

#endif // CLSTRAYITEM_H
