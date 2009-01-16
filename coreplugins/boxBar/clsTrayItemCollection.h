#ifndef CLSTRAYITEMCOLLECTION_H
#define CLSTRAYITEMCOLLECTION_H

#include "clsItemCollection.h"


class clsTrayItemCollection : public clsItemCollection
{
public:
	clsTrayItemCollection(bool pVertical);

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);
protected:
	virtual void populateTray();

	UINT iconSize;
	int numRowCols;
private:
	const char * m_broamPrefix;
	bool m_newFirst;
	bool m_reverseOrder;
};

#endif // CLSTRAYITEMCOLLECTION_H
