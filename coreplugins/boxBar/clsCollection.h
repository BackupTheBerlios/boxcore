#ifndef CLSITEMCOLLECTION_H
#define CLSITEMCOLLECTION_H

#include "clsItem.h"

#include <list>

namespace Plugin_boxBar
{

typedef std::list<Item *> itemList_t;

class Collection : public Item
{
public:
	Collection(bool pVertical, LPCSTR p_itemName = NULL, INT p_defaultBorder = 0, INT p_defaultSpacing = 2, minMaxStruct p_minMax = minMaxStruct());
	virtual ~Collection();
	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void draw(HDC pContext);
	virtual void addItem(Item *p_item, bool p_front = false);
	virtual void calculateSizes(bool pSizeGiven = false);
	virtual void move(int pX, int pY);
	virtual void configMenu(Menu *pMenu, bool p_update = false);
protected:
	void sortItems();
	itemList_t itemList;
	Item *lastMouse;

	INT &spacingBorder;
	INT &spacingItems;

	int flexibleItemCount;
	int fixedItemUsed;
	int flexibleItemSize;

	bool isBar;

	/** @brief Vertical state of the item
	  *
	  * May affect drawing, sizing or calculating functions
	  */
	bool &vertical;
private:
};

}

#endif // CLSITEMCOLLECTION_H
