#ifndef CLSTASKITEMCOLLECTION_H
#define CLSTASKITEMCOLLECTION_H

#include "clsItemCollection.h"
#include <map>


class clsTaskItemCollection : public clsItemCollection
{
public:
	clsTaskItemCollection(bool pVertical);

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu);
protected:
	bool stretchTaskarea;
	void populateTasks();

	std::map<HWND, clsItem *> itemMapping;
private:
};

#endif // CLSTASKITEMCOLLECTION_H
