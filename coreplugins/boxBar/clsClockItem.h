#ifndef CLSCLOCKITEM_H
#define CLSCLOCKITEM_H

#include "clsLabelItem.h"


class clsClockItem : public clsLabelItem
{
public:
	clsClockItem(bool pVertical);
	virtual ~clsClockItem();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
protected:
	UINT ClockTimer;
	TCHAR clockFormat[256];
	TCHAR clockTipFormat[256];
private:
	static void showMenu(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // CLSCLOCKITEM_H
