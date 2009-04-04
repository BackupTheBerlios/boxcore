#ifndef CLSITEMCOLLECTION_H
#define CLSITEMCOLLECTION_H

#include "clsItem.h"

#include <list>

namespace boxBar
{

typedef std::list<clsItem *> itemList_t;

class clsItemCollection : public clsItem
{
public:
	clsItemCollection(bool pVertical, LPCSTR p_itemName = NULL, INT p_defaultBorder = 0, INT p_defaultSpacing = 2, INT &p_maxSizeX = dummyMaxInt);
	virtual ~clsItemCollection();
	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void draw(HDC pContext);
	virtual void addItem(clsItem *p_item, bool p_front = false);
	virtual void calculateSizes(bool pSizeGiven = false);
	virtual void move(int pX, int pY);
	virtual void configMenu(Menu *pMenu, bool p_update = false);
protected:
	void sortItems();
	itemList_t itemList;
	clsItem *lastMouse;

	INT &spacingBorder;
	INT &spacingItems;

	int flexibleItemCount;
	int fixedItemUsed;
	int flexibleItemSize;

	bool isBar;
private:
};

}

#endif // CLSITEMCOLLECTION_H
