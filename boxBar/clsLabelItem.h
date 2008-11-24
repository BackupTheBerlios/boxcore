#ifndef CLSLABELITEM_H
#define CLSLABELITEM_H

#include "clsItem.h"

#include <string>

typedef std::basic_string<TCHAR> tstring;


class clsLabelItem : public clsItem
{
	public:
		clsLabelItem(bool pVertical);
		virtual ~clsLabelItem();
		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void draw(HDC pContext);
		virtual void calculateSizes(bool pSizeGiven = false);
	protected:
		TCHAR text[256];
	private:

};

#endif // CLSLABELITEM_H
