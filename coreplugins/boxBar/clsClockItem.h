#ifndef CLSCLOCKITEM_H
#define CLSCLOCKITEM_H

#include "clsLabel.h"

namespace Plugin_boxBar
{

class clsClockItem : public Label
{
public:
	clsClockItem(bool pVertical);
	virtual ~clsClockItem();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual bool AssignButton(LPCSTR p_data, mouseFunction &p_hook, LPCSTR &p_broamSlot);
protected:
	UINT ClockTimer;
	tstring clockFormat;
	tstring clockTipFormat;
private:
	static void showMenu(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
	static void TimeControlPanel(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
};

}

#endif // CLSCLOCKITEM_H
