#ifndef CLSTRAYITEMCOLLECTION_H
#define CLSTRAYITEMCOLLECTION_H

#include "clsItemCollection.h"

namespace boxBar
{

/** @internal
 * @brief Implements the containter for the system tray
 */
class clsTrayItemCollection : public clsItemCollection
{
public:
	clsTrayItemCollection(bool pVertical, LPCSTR p_itemName = "Tray");

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void writeSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);
protected:
	virtual void populateTray();

	INT &iconSize; ///< @brief The size of tray icons
	INT &numRowCols; ///< @brief The number of rows or columns of icons
private:
	bool m_newFirst; ///< @brief Sets whether icons are added at the beginning or end of the tray
	bool m_reverseOrder; ///< @brief Swaps the beginning and end of the tray
};

}

#endif // CLSTRAYITEMCOLLECTION_H
