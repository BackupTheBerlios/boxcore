#ifndef CLSTASKITEMCOLLECTION_H
#define CLSTASKITEMCOLLECTION_H

#include "clsCollection.h"
#include "clsDropTarget.h"
#include <map>

namespace boxBar
{

class Task;

class TaskArea : public Collection
{
public:
	TaskArea(bool pVertical, LPCSTR p_itemName = "Tasks.Area");
	virtual ~TaskArea();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void readSettings();
	virtual void configMenu(Menu *pMenu, bool p_update = false);
protected:
	bool &stretchTaskarea;
	void populateTasks();

	std::map<HWND, Item *> itemMapping;
private:
	std::string m_basePrefix;
	static void DragAction(Item *p_item, eDragDropState p_state, INT p_x, INT p_y);
	DropTarget *m_dropTarget;
	Task *m_dragTask;
	UINT m_dragTimer;
};

}

#endif // CLSTASKITEMCOLLECTION_H
