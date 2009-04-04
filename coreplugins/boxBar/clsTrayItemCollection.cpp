#include "BBApi.h"
#include "clsTrayItemCollection.h"
#include "clsTrayItem.h"
#include <cstdlib>
#include <tchar.h>

/**
 * @page boxBarRC RC settings for boxBar
 * @section boxBarTray Settings which control the system tray
 * The system tray is a collection item, so settings for @ref boxBarCollection "Collection Items" also apply.
 *
 * @code boxBar.Tray.NumRows: 0 @endcode
 * Number of rows in the tray
 * @note Only used when the tray is in vertical mode
 *
 * @code boxBar.Tray.NumCols: 0 @endcode
 * Number of columns in the tray
 * @note Only used when the tray is not in vertical mode
 *
 * @code boxBar.Tray.Vertical: <bar vertical setting> @endcode
 * Sets whether the tray is drawn vertically or horizontally. Matches the bar setting if not set.
 * You generally want to set this to false when you want multiple rows if icons on a horizontal bar,
 * or set this to true for a vertical bar when you want to restrict the number of columns.
 */

clsTrayItemCollection::clsTrayItemCollection(bool pVertical, LPCSTR p_itemName):clsItemCollection(pVertical, p_itemName, 0, 1),
		iconSize(s_settingsManager.AssociateInt(m_pluginPrefix, m_itemPrefix, "IconSize", 16)),
		numRowCols(s_settingsManager.AssociateInt(m_pluginPrefix, m_itemPrefix, (vertical ? "maxRows" : "maxCols"), 0)),
		m_newFirst(s_settingsManager.AssociateBool(m_pluginPrefix, m_itemPrefix, "NewIconsFirst", false)),
		m_reverseOrder(s_settingsManager.AssociateBool(m_pluginPrefix, m_itemPrefix, "ReverseOrder", false))
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
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
		const char *msg_string = reinterpret_cast<LPCSTR>(lParam);
		LPCSTR element = NULL;
		msg_string += 1;
		if (!strnicmp(msg_string, m_pluginPrefix, strlen(m_pluginPrefix)))
		{
			msg_string += strlen(m_pluginPrefix) + 1;
			if (!strnicmp(msg_string, m_itemPrefix, strlen(m_itemPrefix)))
			{
				msg_string += strlen(m_itemPrefix) + 1;
				if ((element = "Vertical") && !stricmp(msg_string, element))
				{
					vertical = !vertical;
					s_settingsManager.WriteSetting(m_pluginPrefix, m_itemPrefix, element);
					configMenu(NULL, true);
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
				else if ((element = "ReverseOrder") && !stricmp(msg_string, element))
				{
					m_reverseOrder = !m_reverseOrder;
					writeSettings();
					readSettings();
					configMenu(NULL, true);
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				}
				else if ((element = "NewIconsFirst") && !stricmp(msg_string, element))
				{
					m_newFirst = !m_newFirst;
					writeSettings();
					readSettings();
					configMenu(NULL, true);
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
				}
				else if (!strnicmp(msg_string, "maxRows", strlen("maxRows")))
				{
					msg_string += strlen("maxRows");
					numRowCols = atoi(msg_string);
					writeSettings();
					readSettings();
					configMenu(NULL, true);
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
				else if (!strnicmp(msg_string, "maxCols", strlen("maxCols")))
				{
					msg_string += strlen("maxCols");
					numRowCols = atoi(msg_string);
					writeSettings();
					readSettings();
					configMenu(NULL, true);
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
				else if ((element = "IconSize") && !strnicmp(msg_string, element, strlen(element)))
				{
					msg_string += strlen(element);
					iconSize = atoi(msg_string);
					s_settingsManager.WriteSetting(m_pluginPrefix, m_itemPrefix, element);
					configMenu(NULL, true);
					PostMessage(barWnd, BOXBAR_UPDATESIZE, 1, 0);
				}
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
	CHAR buffer[256];
	LPCSTR menuTitle = "Tray Configuration";
	sprintf(buffer, "%s.%s", m_pluginPrefix, m_itemPrefix);
	Menu *subMenu = MakeNamedMenu(menuTitle ,buffer, !p_update);
	if (pMenu)
	{
		MakeSubmenu(pMenu,subMenu,menuTitle);
	}
	MakeMenuItem(subMenu,"Vertical",ItemBroam(buffer, "vertical"),vertical);
	MakeMenuItemInt(subMenu, "Icon Size", ItemBroam(buffer, "iconSize"),iconSize,1,64);
	if (vertical)
	{
		MakeMenuItemInt(subMenu, "Maximum rows", ItemBroam(buffer, "maxRows"),numRowCols,0,255);
	}
	else
	{
		MakeMenuItemInt(subMenu, "Maximum columns", ItemBroam(buffer, "maxCols"),numRowCols,0,255);
	}
	MakeMenuItem(subMenu,"Reverse Order",ItemBroam(buffer, "reverseOrder"),m_reverseOrder);
	MakeMenuItem(subMenu,"New Icons First",ItemBroam(buffer, "newIconsFirst"),m_newFirst);
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
			addItem(new clsItemCollection(!vertical, "TrayInternal", 0, 1));
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
				(dynamic_cast<clsItemCollection*>(*columnRev))->addItem(new clsTrayItem(trayItem, iconSize, vertical));
				columnRev++;
				if (columnRev == itemList.rend())
					columnRev = itemList.rbegin();
			}
			else
			{
				(dynamic_cast<clsItemCollection*>(*column))->addItem(new clsTrayItem(trayItem, iconSize, vertical), m_reverseOrder);
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
				(dynamic_cast<clsItemCollection*>(*columnRev))->addItem(new clsIconItem(NULL, iconSize, vertical));
				columnRev++;
				if (columnRev == itemList.rend())
					columnRev = itemList.rbegin();
			}
			else
			{
				(dynamic_cast<clsItemCollection*>(*column))->addItem(new clsIconItem(NULL, iconSize, vertical), m_reverseOrder);
				column++;
				if (column == itemList.end())
					column = itemList.begin();
			}
		}
	}
}

void clsTrayItemCollection::writeSettings()
{
}

/** @brief Reads settings from the RC file
  *
  * Reads the tray icon size, tray orientation, max number of rows and max number of columns
  */
void clsTrayItemCollection::readSettings()
{
	clsItem::readSettings();
	populateTray();
}



