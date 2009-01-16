#include "BBApi.h"
#include "clsTrayItemCollection.h"
#include "clsTrayItem.h"
#include <cstdlib>

clsTrayItemCollection::clsTrayItemCollection(bool pVertical):clsItemCollection(pVertical)
{
	fixed = DIM_BOTH;
	m_broamPrefix = "@boxBar.tray.";
	readSettings();

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
	case BB_RECONFIGURE:
		readSettings();
		PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
		break;
	case BB_TRAYUPDATE:
		switch (lParam)
		{
		case TRAYICON_ADDED:
		case TRAYICON_REMOVED:
			populateTray();
			SendMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
			return 0;
		case TRAYICON_MODIFIED:
			return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
		}
		break;
	case BB_BROADCAST:
	{
		const char *msg_string = (LPCSTR)lParam;
		if (!strnicmp(msg_string, m_broamPrefix, strlen(m_broamPrefix)))
		{
			msg_string += strlen(m_broamPrefix);
			if (!stricmp(msg_string, "vertical"))
			{
				WriteBool(configFile, "boxBar.tray.vertical:", !vertical);
				readSettings();
				populateTray();
				configMenu(NULL, true);
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
			}
			else if (!stricmp(msg_string, "reverseOrder"))
			{
				WriteBool(configFile, "boxBar.tray.reverseOrder:", !m_reverseOrder);
				readSettings();
				populateTray();
				configMenu(NULL, true);
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
			}
			else if (!stricmp(msg_string, "newIconsFirst"))
			{
				WriteBool(configFile, "boxBar.tray.newIconsFirst:", !m_newFirst);
				readSettings();
				populateTray();
				configMenu(NULL, true);
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
			}
			else if (!strnicmp(msg_string, "maxRows", strlen("maxRows")))
			{
				msg_string += strlen("maxRows");
				WriteInt(configFile, "boxBar.tray.maxRows:",atoi(msg_string));
				readSettings();
				populateTray();
				configMenu(NULL, true);
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
			}
			else if (!strnicmp(msg_string, "maxCols", strlen("maxCols")))
			{
				msg_string += strlen("maxCols");
				WriteInt(configFile, "boxBar.tray.maxCols:",atoi(msg_string));
				readSettings();
				populateTray();
				configMenu(NULL, true);
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
			}
			else if (!strnicmp(msg_string, "iconSize", strlen("iconSize")))
			{
				msg_string += strlen("iconSize");
				WriteInt(configFile, "boxBar.tray.iconSize:",atoi(msg_string));
				readSettings();
				populateTray();
				configMenu(NULL, true);
				PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
			}
		}
		return 0;
	}
	default:
		return clsItemCollection::wndProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
/** @brief populateTray
  *
  * @todo: document this function
  */
void clsTrayItemCollection::configMenu(Menu *pMenu, bool p_update)
{
	Menu *subMenu = MakeNamedMenu("Tray Configuration","boxBar.tray", !p_update);
	if (!p_update)
	{
		MakeSubmenu(pMenu,subMenu,"Tray Configuration");
	}
	MakeMenuItem(subMenu,"Vertical","@boxBar.tray.vertical",vertical);
	MakeMenuItemInt(subMenu, "Icon Size", "@boxBar.tray.iconSize",iconSize,1,64);
	if (vertical)
	{
		MakeMenuItemInt(subMenu, "Maximum rows", "@boxBar.tray.maxRows",numRowCols,0,255);
	}
	else
	{
		MakeMenuItemInt(subMenu, "Maximum columns", "@boxBar.tray.maxCols",numRowCols,0,255);
	}
	MakeMenuItem(subMenu,"Reverse Order","@boxBar.tray.reverseOrder",m_reverseOrder);
	MakeMenuItem(subMenu,"New Icons First","@boxBar.tray.newIconsFirst",m_newFirst);
	if (p_update)
	{
		ShowMenu(subMenu);
	}
}

void clsTrayItemCollection::populateTray()
{
	lastMouse = NULL;
	for (list<clsItem*>::iterator i = itemList.begin(); i != itemList.end(); ++i)
	{
		delete (*i);
	}
	itemList.clear();
	if (numRowCols > 0)
	{
		for (int i=0;i<numRowCols;++i)
			addItem(new clsItemCollection(!vertical));
	}
	list<clsItem*>::iterator column = itemList.begin();
	list<clsItem*>::reverse_iterator columnRev = itemList.rbegin();
	for (int i = (m_newFirst?GetTraySize()-1:0); (m_newFirst?(i>=0):(i < GetTraySize())); (m_newFirst?--i:++i))
	{
		systemTray *trayItem = GetTrayIcon(i);
		if (numRowCols > 0)
		{
			if (m_reverseOrder && !vertical)
			{
				((clsItemCollection*)(*columnRev))->addItem(new clsTrayItem(trayItem, iconSize, vertical));
				columnRev++;
				if (columnRev == itemList.rend())
					columnRev = itemList.rbegin();
			}
			else
			{
				((clsItemCollection*)(*column))->addItem(new clsTrayItem(trayItem, iconSize, vertical), m_reverseOrder);
				column++;
				if (column == itemList.end())
					column = itemList.begin();
			}
		}
		else
			addItem(new clsTrayItem(trayItem, iconSize, vertical), m_reverseOrder);
	}
	if (numRowCols > 0)
	{
		for (int i=GetTraySize(); (i % numRowCols) != 0; ++i)
		{
			if (m_reverseOrder && !vertical)
			{
				((clsItemCollection*)(*columnRev))->addItem(new clsIconItem(NULL, iconSize, vertical));
				columnRev++;
				if (columnRev == itemList.rend())
					columnRev = itemList.rbegin();
			}
			else
			{
				((clsItemCollection*)(*column))->addItem(new clsIconItem(NULL, iconSize, vertical), m_reverseOrder);
				column++;
				if (column == itemList.end())
					column = itemList.begin();
			}
		}
	}
}

/** @brief Reads settings from the RC file
  *
  * Reads the tray icon size, tray orientation, max number of rows and max number of columns
  */
void clsTrayItemCollection::readSettings()
{
	vertical = ReadBool(configFile, "boxBar.tray.vertical:", vertical);
	iconSize = ReadInt(configFile, "boxBar.tray.iconsize:", 16);
	if (vertical)
		numRowCols = ReadInt(configFile, "boxBar.tray.maxRows:", 0);
	else
		numRowCols = ReadInt(configFile, "boxBar.tray.maxCols:", 0);
	spacingBorder = ReadInt(configFile, "boxBar.tray.spacingBorder:", 0);
	spacingItems = ReadInt(configFile, "boxBar.tray.spacingItems:", 2);
	m_newFirst = ReadBool(configFile, "boxBar.tray.newIconsFirst:",false);
	m_reverseOrder = ReadBool(configFile, "boxBar.tray.reverseOrder:", false);
	populateTray();
}



