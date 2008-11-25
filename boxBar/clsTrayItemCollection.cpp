#include "BBApi.h"
#include "clsTrayItemCollection.h"
#include "clsTrayItem.h"

clsTrayItemCollection::clsTrayItemCollection(bool pVertical):clsItemCollection(pVertical)
{
	fixed = DIM_BOTH;
	spacingBorder = 0;
	spacingItems = 2;
	vertical = ReadBool(configFile, "boxBar.tray.vertical:", vertical);

	populateTray();
}

/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsTrayItemCollection::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case BB_TRAYUPDATE:
			switch(lParam)
			{
				case TRAYICON_ADDED:
				case TRAYICON_REMOVED:
					populateTray();
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
					return 0;
				case TRAYICON_MODIFIED:
					return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
			}
			break;
		default:
			return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
/** @brief populateTray
  *
  * @todo: document this function
  */
void clsTrayItemCollection::populateTray()
{
	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		delete (*i);
	}
	itemList.clear();
	for(int i = 0; i < GetTraySize(); ++i)
	{
		systemTray *trayItem = GetTrayIcon(i);
		addItem(new clsTrayItem(trayItem, vertical));
	}
}



