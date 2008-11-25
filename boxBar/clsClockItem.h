#ifndef CLSCLOCKITEM_H
#define CLSCLOCKITEM_H

#include "clsLabelItem.h"


class clsClockItem : public clsLabelItem
{
	public:
		clsClockItem(bool pVertical);
		virtual ~clsClockItem();

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	protected:
		UINT ClockTimer;
		TCHAR clockFormat[256];
	private:
};

#endif // CLSCLOCKITEM_H
