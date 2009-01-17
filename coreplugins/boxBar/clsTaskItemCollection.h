#ifndef CLSTASKITEMCOLLECTION_H
#define CLSTASKITEMCOLLECTION_H

#include "clsItemCollection.h"
#include "clsDropTarget.h"
#include <map>

class clsTaskItem;

class clsTaskItemCollection : public clsItemCollection
{
public:
	clsTaskItemCollection(bool pVertical);
	virtual ~clsTaskItemCollection();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);
protected:
	bool stretchTaskarea;
	void populateTasks();

	std::map<HWND, clsItem *> itemMapping;
private:
	static void DragAction(clsItem *p_item, eDragDropState p_state, INT p_x, INT p_y);
	DropTarget *m_dropTarget;
	clsTaskItem *m_dragTask;
	UINT m_dragTimer;
};

#endif // CLSTASKITEMCOLLECTION_H
