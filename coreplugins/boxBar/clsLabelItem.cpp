#include "BBApi.h"
#include "clsLabelItem.h"
#include <tchar.h>

namespace Plugin_boxBar
{

clsLabelItem::clsLabelItem(bool pVertical, LPCSTR p_itemName) : Collection(pVertical, p_itemName, 2, 0)
{
	m_knowsSize = DIM_BOTH;
	m_wantsStretch = DIM_NONE;
	style = SN_TOOLBARLABEL;
	m_textItem = new Text("Label", 0, pVertical, DIM_BOTH);
	addItem(m_textItem);
}

}
