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
	clsTaskItem(HWND p_Task, bool pVertical);
	~clsTaskItem();

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

	static bool s_activeBackground;
	static bool s_inactiveBackground;

	static int inactiveStyle;
	static int activeStyle;
	static int activeAlpha;
	static int inactiveAlpha;

	bool &m_showIcon;
	bool &m_showText;
private:
	static void activateTask(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
	static void WindowMenu(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);

	static VOID CALLBACK SmallIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult);
	static VOID CALLBACK LargeIconProc(HWND p_hWnd, UINT p_uMsg, ULONG_PTR p_dwData, LRESULT p_lResult);
};

#endif // CLSTASKITEM_H
