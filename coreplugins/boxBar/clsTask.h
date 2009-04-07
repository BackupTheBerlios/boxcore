#ifndef CLSTASKITEM_H
#define CLSTASKITEM_H

#include "clsItemCollection.h"
#include "clsTextItem.h"
#include "clsIconItem.h"
#include "BBApi.h"
#include <tchar.h>

namespace Plugin_boxBar
{

class Task : public clsItemCollection
{
public:
	Task(HWND p_Task, bool pVertical, LPCSTR p_itemName = "Tasks");
	~Task();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);

	const HWND GetTaskWnd()
	{
		return taskWnd;
	}
protected:
	HWND taskWnd;
	TCHAR m_caption[256];

	UINT &iconSize;

	clsTextItem *captionItem;
	clsIconItem *iconItem;

	bool m_fallback;

	bool &m_activeBackground;
	bool &m_inactiveBackground;

	static UINT inactiveStyle;
	static UINT activeStyle;
	static UINT activeAlpha;
	static UINT inactiveAlpha;

	bool &m_showIcon;
	bool &m_showText;
private:
	static void activateTask(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
	static void WindowMenu(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);

	static VOID CALLBACK SmallIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult);
	static VOID CALLBACK LargeIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult);
};

}

#endif // CLSTASKITEM_H
