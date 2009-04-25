#ifndef CLSWORKSPACELABEL_H
#define CLSWORKSPACELABEL_H

#include "clsLabel.h"

namespace Plugin_boxBar
{

class clsWorkspaceLabel : public Label
{
public:
	clsWorkspaceLabel(bool pVertical);
	virtual ~clsWorkspaceLabel();

	virtual LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	static void nextWorkspace(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
	static void prevWorkspace(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam);
private:
};

}

#endif // CLSWORKSPACELABEL_H
