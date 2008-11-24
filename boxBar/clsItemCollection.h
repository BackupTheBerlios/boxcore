#ifndef CLSITEMCOLLECTION_H
#define CLSITEMCOLLECTION_H

#include "clsItem.h"

#include <list>

using std::list;

class clsItemCollection : public clsItem
{
	public:
		clsItemCollection(bool pVertical);
		virtual ~clsItemCollection();
		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void draw(HDC pContext);
		virtual void addItem(clsItem *pItem);
		virtual void calculateSizes(bool pSizeGiven = false);
		virtual void move(int pX, int pY);
	protected:
		void sortItems();
		list<clsItem *> itemList;
		clsItem *lastMouse;

		UINT spacingWith;
		UINT spacingPerp;

		int flexibleItemCount;
		int fixedItemUsed;
		int flexibleItemSize;
	private:
};

#endif // CLSITEMCOLLECTION_H
