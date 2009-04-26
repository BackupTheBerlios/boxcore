#include "clsWorkspaceLabel.h"

namespace boxBar
{

/**
 * @page boxBarRC
 * @section boxBarWSLabel WSLabel (Workspace Label)
 * The workspace label displays the current workspace name and can be used
 * to switch to the next or previous workspace. There are currently no settings
 * available.
 */

clsWorkspaceLabel::clsWorkspaceLabel(bool pVertical): Label(pVertical)
{
	static int msgs[] = {BB_DESKTOPINFO, 0};
	PostMessage(hBlackboxWnd, BB_REGISTERMESSAGE, reinterpret_cast<WPARAM>(barWnd), reinterpret_cast<LPARAM>(msgs));
	DesktopInfo deskinfo;
	GetDesktopInfo(&deskinfo);
	m_textItem->SetText(deskinfo.name);
	//CopyString(text, deskinfo.name, 256);
	leftClick = nextWorkspace;
	rightClick = prevWorkspace;
}

clsWorkspaceLabel::~clsWorkspaceLabel()
{
	static int msgs[] = {BB_DESKTOPINFO, 0};
	SendMessage(hBlackboxWnd, BB_UNREGISTERMESSAGE, reinterpret_cast<WPARAM>(barWnd), reinterpret_cast<LPARAM>(msgs));
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
		DesktopInfo *deskinfo = reinterpret_cast<DesktopInfo *>(lParam);
		m_textItem->SetText(deskinfo->name);
		PostMessage(barWnd, BOXBAR_UPDATESIZE, 0, 0);
		return 0;
	}
	}
	return Label::wndProc(hWnd, msg, wParam, lParam);
}

/** @brief nextWorkspace
  *
  * @todo: document this function
  */
void clsWorkspaceLabel::nextWorkspace(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hBlackboxWnd, BB_WORKSPACE, BBWS_DESKRIGHT, 0);
}

/** @brief prevWorkspace
  *
  * @todo: document this function
  */
void clsWorkspaceLabel::prevWorkspace(Item *pItem, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostMessage(hBlackboxWnd, BB_WORKSPACE, BBWS_DESKLEFT, 0);
}

}
