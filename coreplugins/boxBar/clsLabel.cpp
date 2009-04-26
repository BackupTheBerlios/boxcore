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

void Label::SetStyle(UINT p_style)
{
	style = p_style;
	m_textItem->setStyle(p_style);
}

}
