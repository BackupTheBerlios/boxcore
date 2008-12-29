#include "clsWorkspaceLabel.h"

clsWorkspaceLabel::clsWorkspaceLabel(bool pVertical): clsLabelItem(pVertical)
{
	static int msgs[] = {BB_DESKTOPINFO, 0};
	PostMessage(hBlackboxWnd, BB_REGISTERMESSAGE, (WPARAM)barWnd, (LPARAM)msgs);
	DesktopInfo deskinfo;
	GetDesktopInfo(&deskinfo);
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, deskinfo.name, -1, text, 256);
#else
	strcpy(text, deskinfo.name);
#endif
	leftClick = nextWorkspace;
	rightClick = prevWorkspace;
}

clsWorkspaceLabel::~clsWorkspaceLabel()
{
	static int msgs[] = {BB_DESKTOPINFO, 0};
	SendMessage(hBlackboxWnd, BB_UNREGISTERMESSAGE, (WPARAM)barWnd, (LPARAM)msgs);
}


/** @brief wndProc
  *
  * @todo: document this function
  */
LRESULT clsWorkspaceLabel::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case BB_DESKTOPINFO:
	{
		DesktopInfo *deskinfo = (DesktopInfo *)lParam;
#ifdef UNICODE
		MultiByteToWideChar(CP_ACP, 0, deskinfo->name, -1, text, 256);
#else
		strcpy(text, deskinfo->name);
#endif
		PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
		return 0;
	}
	}
	return clsLabelItem::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief nextWorkspace
  *
  * @todo: document this function
  */
void clsWorkspaceLabel::nextWorkspace(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hBlackboxWnd, BB_WORKSPACE, BBWS_DESKRIGHT, 0);
}

/** @brief prevWorkspace
  *
  * @todo: document this function
  */
void clsWorkspaceLabel::prevWorkspace(clsItem *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hBlackboxWnd, BB_WORKSPACE, BBWS_DESKLEFT, 0);
}



