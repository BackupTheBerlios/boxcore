#ifndef CLSWORKSPACELABEL_H
#define CLSWORKSPACELABEL_H

#include "clsLabelItem.h"


class clsWorkspaceLabel : public clsLabelItem
{
	public:
		clsWorkspaceLabel(bool pVertical);
		virtual ~clsWorkspaceLabel();

		virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	protected:
		static void nextWorkspace(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
		static void prevWorkspace(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
};

#endif // CLSWORKSPACELABEL_H
