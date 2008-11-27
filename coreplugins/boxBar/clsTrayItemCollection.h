#ifndef CLSTRAYITEMCOLLECTION_H
#define CLSTRAYITEMCOLLECTION_H

#include "clsItemCollection.h"


class clsTrayItemCollection : public clsItemCollection
{
	public:
		clsTrayItemCollection(bool pVertical);

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	protected:
		virtual void populateTray();

		int numRowCols;
	private:
};

#endif // CLSTRAYITEMCOLLECTION_H
