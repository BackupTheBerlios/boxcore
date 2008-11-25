#ifndef CLSTASKITEMCOLLECTION_H
#define CLSTASKITEMCOLLECTION_H

#include "clsItemCollection.h"


class clsTaskItemCollection : public clsItemCollection
{
	public:
		clsTaskItemCollection(bool pVertical);

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	protected:
		void populateTasks();
	private:
};

#endif // CLSTASKITEMCOLLECTION_H
