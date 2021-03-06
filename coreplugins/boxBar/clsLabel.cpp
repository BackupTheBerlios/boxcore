#include "BBApi.h"
#include "clsLabel.h"
#include <tchar.h>

namespace boxBar
{

Label::Label(bool pVertical, LPCSTR p_itemName) : Collection(pVertical, p_itemName, 2, 0)
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
	style = SN_TOOLBARLABEL;
	m_textItem = new Text("Label", 0, DIM_BOTH);
	addItem(m_textItem);
}

LRESULT Label::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Item::wndProc(hWnd, msg, wParam, lParam);
}

void Label::SetStyle(UINT p_style)
{
	style = p_style;
	m_textItem->SetStyle(p_style);
}

}
