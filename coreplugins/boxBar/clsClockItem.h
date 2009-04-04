#ifndef CLSCLOCKITEM_H
#define CLSCLOCKITEM_H

#include "clsLabelItem.h"

namespace Plugin_boxBar
{

class clsClockItem : public clsLabelItem
{
public:
	clsClockItem(bool pVertical);
	virtual ~clsClockItem();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual bool AssignButton(LPCSTR p_data, mouseFunction &p_hook, LPCSTR &p_broamSlot);
protected:
	UINT ClockTimer;
	TCHAR clockFormat[256];
	TCHAR clockTipFormat[256];
private:
	static void showMenu(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
	static void TimeControlPanel(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
};

}

#endif // CLSCLOCKITEM_H
