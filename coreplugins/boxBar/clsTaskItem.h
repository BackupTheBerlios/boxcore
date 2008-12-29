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
	~clsTaskItem();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu);

	const HWND GetTaskWnd()
	{
		return taskWnd;
	}
protected:
	HWND taskWnd;
	TCHAR caption[256];

	UINT iconSize;

	clsTextItem *captionItem;
	clsIconItem *iconItem;

	static int inactiveStyle;
	static int activeStyle;
	static int activeAlpha;
	static int inactiveAlpha;
private:
	static void activateTask(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // CLSTASKITEM_H
