#ifndef CLSTASKITEM_H
#define CLSTASKITEM_H

#include "clsItemCollection.h"
#include "clsTextItem.h"
#include "clsIconItem.h"
#include "BBApi.h"
#include <tchar.h>


class clsTaskItem : public clsItemCollection
{
	public:
		clsTaskItem(tasklist *pTask, bool pVertical);
		virtual ~clsTaskItem();

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void draw(HDC pContext);
	protected:
		HWND taskWnd;
		TCHAR caption[256];

		clsTextItem *captionItem;
		clsIconItem *iconItem;

		int style;
	private:
};

#endif // CLSTASKITEM_H
