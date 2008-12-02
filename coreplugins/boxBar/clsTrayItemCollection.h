#ifndef CLSTRAYITEMCOLLECTION_H
#define CLSTRAYITEMCOLLECTION_H

#include "clsItemCollection.h"


class clsTrayItemCollection : public clsItemCollection
{
	public:
		clsTrayItemCollection(bool pVertical);

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual void readSettings();
	protected:
		virtual void populateTray();

		UINT iconSize;
		int numRowCols;
	private:
};

#endif // CLSTRAYITEMCOLLECTION_H
