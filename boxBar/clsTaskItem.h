#ifndef CLSTASKITEM_H
#define CLSTASKITEM_H

#include "clsItem.h"
#include "BBApi.h"
#include <tchar.h>


class clsTaskItem : public clsItem
{
	public:
		clsTaskItem(tasklist *pTask, bool pVertical);
		virtual ~clsTaskItem();

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void draw(HDC pContext);
		virtual void calculateSizes(bool pSizeGiven = false);
	protected:
		TCHAR caption[256];
	private:
};

#endif // CLSTASKITEM_H
