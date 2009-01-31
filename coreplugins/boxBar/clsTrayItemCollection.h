#ifndef CLSTRAYITEMCOLLECTION_H
#define CLSTRAYITEMCOLLECTION_H

#include "clsItemCollection.h"
#include "rcworker/clsRCWorker.h"
#include <vector>


class clsTrayItemCollection : public clsItemCollection
{
public:
	clsTrayItemCollection(bool pVertical);

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void writeSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);
protected:
	virtual void populateTray();

	INT iconSize;
	int numRowCols;
private:
	bool m_newFirst;
	bool m_reverseOrder;
};

#endif // CLSTRAYITEMCOLLECTION_H
